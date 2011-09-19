//
// LVModuleManager.mm
//
// Copyright (c) 2004-2009 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#import "LVConfig.h"
#import "LVModuleManager.h"
#include <OpenVanilla/OVWildcard.h>

using namespace OpenVanilla;

static NSString *LVPrimaryInputMethodKey = @"primaryInputMethod";
static NSString *LVActivatedOutputFiltersKey = @"activatedOutputFilters";

NSString *LVModuleConfigChangedNotification = @"LVModuleConfigChangedNotification";

// we need this stuff otherwise funny things would happen... 
// http://www.cocoadev.com/index.pl?MutableDeepCopyAndUserDefaults


@interface NSObject (MutableDeepCopy)
- (id)mutableDeepCopy;
@end

@implementation NSObject (MutableDeepCopy)
- (id)mutableDeepCopy
{
    if([self respondsToSelector:@selector(mutableCopyWithZone:)])
        return [self mutableCopy];
    else if([self respondsToSelector:@selector(copyWithZone:)])
        return [self copy];
    else
        return [self retain];
}
@end

@implementation NSArray (MutableDeepCopy)
- (id)mutableDeepCopy
{
    NSMutableArray *newArray = [[NSMutableArray alloc] init];
    NSEnumerator *enumerator = [self objectEnumerator];
    id obj;
    while((obj = [enumerator nextObject]))
    {
        obj = [obj mutableDeepCopy];
        [newArray addObject:obj];
        [obj release];
    }
    return newArray;
}
@end

@implementation NSSet (MutableDeepCopy)
- (id)mutableDeepCopy
{
    NSMutableSet *newSet = [[NSMutableSet alloc] init];
    NSEnumerator *enumerator = [self objectEnumerator];
    id obj;
    while((obj = [enumerator nextObject]))
    {
        obj = [obj mutableDeepCopy];
        [newSet addObject:obj];
        [obj release];
    }
    return newSet;
}
@end

@implementation NSDictionary (MutableDeepCopy)
- (id)mutableDeepCopy
{
    NSMutableDictionary *newDictionary = [[NSMutableDictionary alloc] init];
    NSEnumerator *enumerator = [self keyEnumerator];
    id key;
    while((key = [enumerator nextObject]))
    {
        id obj = [[self objectForKey:key] mutableDeepCopy];
        [newDictionary setObject:obj forKey:key];
        [obj release];
    }
    return newDictionary;
}
@end


@implementation LVModule
- (void)dealloc
{
	delete _module;
	[_moduleDataPath release];
	[super dealloc];
}
- (id)initWithModule:(OVModule *)module moduleDataPath:(NSString *)dataPath
{
	if (self = [super init]) {
		_module = module;
		_moduleDataPath = [dataPath copy];
	}
	return self;
}
+ (LVModule *)moduleWithModuleObject:(OVModule*)module moduleDataPath:(NSString *)dataPath
{
	return [[[LVModule alloc] initWithModule:module moduleDataPath:(NSString *)dataPath] autorelease];
}
- (OVModule *)moduleObject
{
	return _module;
}
- (NSString *)description
{
	return [NSString stringWithFormat:@"OVModule (type '%s', identifier '%s', en name '%s')", _module->moduleType(), _module->identifier(), _module->localizedName("en")];
}

- (NSString *)moduleIdentifier
{
	const char *i = _module->identifier();
	return [NSString stringWithUTF8String:_module->identifier()];
}

- (BOOL)lazyInitWithLoaderService:(LVService*)service configDictionary:(NSMutableDictionary *)configDict
{
	if (_initialized) {
		return _usable;
	}
	
	
	LVDictionary cd(configDict);
	
	_initialized = YES;
	_usable = !!_module->initialize(&cd, service, [_moduleDataPath UTF8String]);
		
	return _usable;
}
- (BOOL)isInitialized
{
	return _initialized;
}
- (BOOL)isUsable
{
	if (!_initialized) {
		return YES;
	}
	return _usable;
}
@end


@implementation LVModuleManager : NSObject
- (NSString *)_configFilePath
{
	NSArray *dirs = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    NSAssert([dirs count], @"NSSearchPathForDirectoriesInDomains");		
	NSString *prefPath = [[dirs objectAtIndex:0] stringByAppendingPathComponent:@"Preferences"];	
	return [prefPath stringByAppendingPathComponent:OPENVANILLA_LOADER_PLIST_FILENAME];
}
- (void)_unloadEverything
{
	[_loadedModuleDictionary removeAllObjects];
	
	NSEnumerator *keyEnum = [_loadedModulePackageBundleDictionary keyEnumerator];	
	NSString *key;
	while (key = [keyEnum nextObject]) {
		CFBundleRef bundle = (CFBundleRef)[_loadedModulePackageBundleDictionary objectForKey:key];
		CFBundleUnloadExecutable(bundle);
	}
	[_loadedModulePackageBundleDictionary removeAllObjects];
}
- (void)_writeConfigurationFile
{
	NSData *data = [NSPropertyListSerialization dataFromPropertyList:_configDictionary format:NSPropertyListXMLFormat_v1_0 errorDescription:nil];
	if (data) {
		[data writeToFile:[self _configFilePath] atomically:YES];
		
		NSDictionary *attr = [[NSFileManager defaultManager] fileAttributesAtPath:[self _configFilePath] traverseLink:YES];
		_configTimestamp = [[attr objectForKey:NSFileModificationDate] timeIntervalSince1970];
	}
}

- (void)_validateAndWriteConfig
{
	// validate the config
	if (![_primaryInputMethodModuleID length]) {
		NSArray *allInputMethods = [self inputMethodTitlesAndModuleIDs];
		if ([allInputMethods count]) {
			[_primaryInputMethodModuleID autorelease];
			_primaryInputMethodModuleID = [[[allInputMethods objectAtIndex:0] objectAtIndex:1] retain];
		}
	}
	
	if ([_primaryInputMethodModuleID length]) {
		LVModule *module = [_loadedModuleDictionary objectForKey:_primaryInputMethodModuleID];
		
		BOOL valid = NO;
		if (module) {
			if (OVWildcard::Match([module moduleObject]->moduleType(), "OVInputMethod")) {
				valid = YES;
			}
		}
		
		if (!valid) {
			[_primaryInputMethodModuleID autorelease];
			_primaryInputMethodModuleID = nil;
		}
	}
	
	NSMutableArray *fa = [NSMutableArray array];
	NSEnumerator *fe = [_activatedOutputFilterModuleIDs objectEnumerator];
	NSString *fi;
	while (fi = [fe nextObject]) {
		LVModule *module = [_loadedModuleDictionary objectForKey:fi];
		
		BOOL valid = NO;
		if (module) {
			if (OVWildcard::Match([module moduleObject]->moduleType(), "OVOutputFilter")) {
				valid = YES;
			}
		}
		
		if (valid) {
			[fa addObject:fi];
		}		
	}
	
	[_activatedOutputFilterModuleIDs removeAllObjects];
	[_activatedOutputFilterModuleIDs addObjectsFromArray:fa];
	
	[_configDictionary setObject:_activatedOutputFilterModuleIDs forKey:LVActivatedOutputFiltersKey];
	[_configDictionary setObject:([_primaryInputMethodModuleID length] ? _primaryInputMethodModuleID : @"") forKey:LVPrimaryInputMethodKey];
	[self _writeConfigurationFile];	
}

- (void)_notify:(BOOL)sendNotification
{
	// tell every usable module that some configuration has been changed
	
	NSEnumerator *keyEnum = [_loadedModuleDictionary keyEnumerator];
	NSString *moduleIdentifier;
	while (moduleIdentifier = [keyEnum nextObject]) {
		LVModule *module = [_loadedModuleDictionary objectForKey:moduleIdentifier];
		if ([module isInitialized]  && [module isUsable]) {
			OVModule *moduleObject = [module moduleObject];
			NSMutableDictionary *configDict = [_configDictionary objectForKey:[module moduleIdentifier]];
			if (!configDict) {
				configDict = [NSMutableDictionary dictionary];
				[_configDictionary setObject:configDict forKey:[module moduleIdentifier]];
			}		
			
			LVDictionary dict(configDict);
			moduleObject->update(&dict, _loaderService);
		}
	}
	
	if (sendNotification) {
		[[NSNotificationCenter defaultCenter] postNotificationName:LVModuleConfigChangedNotification object:self];
	}	
}

- (void)_syncConfigurationWithNotification:(BOOL)sendNotification
{
	NSDictionary *oldConfigDict = [[_configDictionary mutableDeepCopy] autorelease];
	
	// get a list of all keys
	NSDictionary *attr = [[NSFileManager defaultManager] fileAttributesAtPath:[self _configFilePath] traverseLink:YES];
	NSTimeInterval nowTimestamp = [[attr objectForKey:NSFileModificationDate] timeIntervalSince1970];
	
	if (!attr) {
		// file doesn't exist (1st run)--force write config file and quit
		[self _validateAndWriteConfig];
		
		// set this to zero to ensure next write-sync (because our time resolution is 1 sec which is too coarse-grained)
		_configTimestamp = 0;
		
		return;
	}
	
	if (_configTimestamp == nowTimestamp) {
		return;
	}
	
	_configTimestamp = nowTimestamp;
	
	NSData *configData = [NSData dataWithContentsOfFile:[self _configFilePath]];
	if (configData) {
		NSPropertyListFormat format;
		id plist = [NSPropertyListSerialization propertyListFromData:configData mutabilityOption:NSPropertyListMutableContainersAndLeaves format:&format errorDescription:nil];
		if ([plist isKindOfClass:[NSDictionary class]]) {
			[_configDictionary removeAllObjects];
			[_configDictionary addEntriesFromDictionary:plist];
		}
	}
	
	_primaryInputMethodModuleID = [[_configDictionary objectForKey:LVPrimaryInputMethodKey] retain];
	[_activatedOutputFilterModuleIDs removeAllObjects];
	
	NSArray *filterIDs = [_configDictionary objectForKey:LVActivatedOutputFiltersKey];
	if ([filterIDs isKindOfClass:[NSArray class]]) {
		[_activatedOutputFilterModuleIDs addObjectsFromArray:filterIDs];
	}	
		
	if (![oldConfigDict isEqualToDictionary:_configDictionary]) {
		[self _notify:sendNotification];
	}

	[self _validateAndWriteConfig];
}

- (void)delloc
{
    [self _unloadEverything];
	[_loadedModuleDictionary release];
    [_loadedModulePackageBundleDictionary release];
    [_modulePackageBundlePaths release];	
	[_configDictionary release];
	[_primaryInputMethodModuleID release];
	[_activatedOutputFilterModuleIDs release];
	[_distributedObjectPort release];
	delete _loaderService;
	
    [super dealloc];
}
- (id)init
{
    if (self = [super init]) {
		_configDictionary = [NSMutableDictionary new];
        _modulePackageBundlePaths = [NSMutableArray new];
        _loadedModulePackageBundleDictionary = [NSMutableDictionary new];
		_loadedModuleDictionary = [NSMutableDictionary new];
		_activatedOutputFilterModuleIDs = [NSMutableArray new];
		
		_loaderService = new LVService;
		
	    NSArray *languages = [[NSUserDefaults standardUserDefaults] objectForKey:@"AppleLanguages"];
		if ([languages count]) {
			_loaderService->setLocale([[languages objectAtIndex:0] UTF8String]);
		}
		else {
			_loaderService->setLocale("en");
		}
		
		_distributedObjectPort = [[NSPort port] retain];
		_distributedObjectConnection  = [[NSConnection connectionWithReceivePort:_distributedObjectPort sendPort:_distributedObjectPort] retain];	
		[_distributedObjectConnection setRootObject:self];
		
		if ([_distributedObjectConnection registerName:LVDOINTERFACE_PROTOCOL_NAME]) {
		}
		else {
			NSLog(@"Failed to register DO service named: %@", LVDOINTERFACE_PROTOCOL_NAME);
		}		
		
    }
    return self;
}
+ (LVModuleManager *)sharedManager
{
    static LVModuleManager *sharedInstance = nil;
    if (!sharedInstance) {
        sharedInstance = [LVModuleManager new];
    }
    return sharedInstance;
}
- (void)setModulePackageBundlePaths:(NSArray *)array
{
    [_modulePackageBundlePaths setArray:array];
}
- (void)loadModulePackageBundles
{
    [self _unloadEverything];
    
	NSMutableArray *_bundlePathArray = [NSMutableArray array];
	
    NSEnumerator *mpbpEnum = [_modulePackageBundlePaths objectEnumerator];
    NSString *path;
    while (path = [mpbpEnum nextObject]) {
        if (![[NSFileManager defaultManager] fileExistsAtPath:path]) {
            continue;
        }
        
        NSDirectoryEnumerator *dirEnum = [[NSFileManager defaultManager] enumeratorAtPath:path];
        NSString *target;
        while (target = [dirEnum nextObject]) {
            if ([[target pathExtension] isEqualToString:@"bundle"]) {
                [dirEnum skipDescendents];
                [_bundlePathArray addObject:[path stringByAppendingPathComponent:target]];
            }
        }
    }
	
	// now try to load everything
	NSEnumerator *bpaEnum = [_bundlePathArray objectEnumerator];
	while (path = [bpaEnum nextObject]) {
		CFBundleRef bundle = CFBundleCreate(NULL, (CFURLRef)[NSURL fileURLWithPath:path]);
		BOOL loaded = NO;
		if (bundle) {
			if (CFBundleLoadExecutable(bundle)) {
				// see if this is what we want...
				_OVGetLibraryVersion_t *getVersion = (_OVGetLibraryVersion_t *)CFBundleGetFunctionPointerForName(bundle, CFSTR("OVGetLibraryVersion"));
				_OVInitializeLibrary_t *initLib = (_OVInitializeLibrary_t *)CFBundleGetFunctionPointerForName(bundle, CFSTR("OVInitializeLibrary"));
				_OVGetModuleFromLibrary_t *getModule = (_OVGetModuleFromLibrary_t *)CFBundleGetFunctionPointerForName(bundle, CFSTR("OVGetModuleFromLibrary"));
				
				if (getVersion && initLib && getModule) {
					if (getVersion() == OV_VERSION) {
						NSString *resourceDir = [[[path stringByAppendingPathComponent:@"Contents"] stringByAppendingPathComponent:@"Resources"] stringByAppendingString:@"/"];
						if (initLib(_loaderService, [resourceDir UTF8String])) {
							size_t moduleIterator = 0;
							OVModule *module;
							while (module = getModule(moduleIterator)) {
								LVModule *loadedModule = [LVModule moduleWithModuleObject:module moduleDataPath:resourceDir];
								[_loadedModuleDictionary setObject:loadedModule forKey:[loadedModule moduleIdentifier]];
								moduleIterator++;
							}
							
							if (moduleIterator)
								loaded = YES;
						}
					}
				}
			}				
		}
	
		if (loaded) {
			[_loadedModulePackageBundleDictionary setObject:(id)bundle forKey:path];
		}
		else {
			if (bundle) {
				CFBundleUnloadExecutable(bundle);
			}
		}
		
		if (bundle) {
			CFRelease(bundle);
		}
	}
	
	[self _syncConfigurationWithNotification:YES];
}
- (LVService*)loaderService
{
	return _loaderService;
}
- (LVContextSandwich *)createContextSandwich
{
	OVInputMethodContext* inputMethodContext = 0;

	if ([_primaryInputMethodModuleID length]) {
		LVModule *module = [_loadedModuleDictionary objectForKey:_primaryInputMethodModuleID];

		NSDictionary *oldConfigDict = [[_configDictionary mutableDeepCopy] autorelease];		
		NSMutableDictionary *configDict = [_configDictionary objectForKey:_primaryInputMethodModuleID];
		if (!configDict) {
			configDict = [NSMutableDictionary dictionary];
			[_configDictionary setObject:configDict forKey:_primaryInputMethodModuleID];
		}
		if ([module lazyInitWithLoaderService:_loaderService configDictionary:configDict]) {
			inputMethodContext = ((OVInputMethod*)[module moduleObject])->newContext();
			
			if (![oldConfigDict isEqualTo:_configDictionary]) {
				[self _writeConfigurationFile];
			}
		}
	}
	
	LVContextSandwich* sandwich = new LVContextSandwich(inputMethodContext);
	return sandwich;
}
- (void)syncConfiguration
{
	[self _syncConfigurationWithNotification:YES];
}

- (LVModule *)moduleForIdentifier:(NSString *)identifier
{
	return [_loadedModuleDictionary objectForKey:identifier];
}

- (void)setPrimaryInputMethodModuleID:(NSString *)moduleID
{
	NSString *tmp = _primaryInputMethodModuleID;
	_primaryInputMethodModuleID = [moduleID copy];
	[tmp release];
	[self _validateAndWriteConfig];
}
- (NSString *)primaryInputMethodModuleID
{
	return _primaryInputMethodModuleID;
}

- (NSArray *)_moduleTitlesAndIDsByType:(const char*)type
{
	NSMutableArray *result = [NSMutableArray array];
	NSEnumerator *keyEnum = [_loadedModuleDictionary keyEnumerator];
	NSString *key;
	
	while (key = [keyEnum nextObject]) {
		LVModule *module = [_loadedModuleDictionary objectForKey:key];		
		if (OVWildcard::Match([module moduleObject]->moduleType(), type)) {		
			NSString *localizedName = [NSString stringWithUTF8String:[module moduleObject]->localizedName(_loaderService->locale())];
			[result addObject:[NSArray arrayWithObjects:localizedName, key, nil]];
		}
	}
	
	return result;	
}

- (NSArray *)inputMethodTitlesAndModuleIDs
{
	return [self _moduleTitlesAndIDsByType:"OVInputMethod"];
}

- (NSArray *)outputFilterTitlesAndModuleIDs
{
	return [self _moduleTitlesAndIDsByType:"OVOutputFilter"];
}

- (BOOL)isOutputFilterActivated:(NSString *)outputFilterID
{
	return [_activatedOutputFilterModuleIDs containsObject:outputFilterID];
}

- (void)toggleOutputFilterModuleID:(NSString *)outputFilterID
{	
	if ([_activatedOutputFilterModuleIDs containsObject:outputFilterID]) {
		[_activatedOutputFilterModuleIDs removeObject:outputFilterID];
	}
	else {
		NSArray *outputFilters = [self outputFilterTitlesAndModuleIDs];
		NSEnumerator *ofe = [outputFilters objectEnumerator];
		NSMutableArray *fa = [NSMutableArray array];
		
		NSArray *oftmi;
		while (oftmi = [ofe nextObject]) {
			NSString *ofid = [oftmi objectAtIndex:1];
			if ([ofid isEqualToString:outputFilterID]) {
				[fa addObject:ofid];
			}
			else if ([_activatedOutputFilterModuleIDs containsObject:ofid]) {
				[fa addObject:ofid];
			}
		}
		
		[_activatedOutputFilterModuleIDs removeAllObjects];
		[_activatedOutputFilterModuleIDs addObjectsFromArray:fa];
	}
	
	[self _validateAndWriteConfig];
}

- (NSMutableDictionary *)configDictionary
{
	return _configDictionary;
}

- (void)forceSyncConfiguration
{
	[self _writeConfigurationFile];
	[self _notify:YES];
}

- (string)processStringWithOutputFilters:(const string &)inputString
{
	if (![_activatedOutputFilterModuleIDs count]) {
		return inputString;
	}
	
	NSEnumerator *ofe = [_activatedOutputFilterModuleIDs objectEnumerator];
	NSString *ofi;
	
	string currentInput = inputString;
	while (ofi = [ofe nextObject]) {
		LVModule *module = [_loadedModuleDictionary objectForKey:ofi];		
		OVOutputFilter *filter = (OVOutputFilter *)[module moduleObject];
		currentInput = filter->process(currentInput.c_str(), _loaderService);
	}
	
	return currentInput;
}

#pragma mark LVDOInterface

- (NSArray *)loadedModuleList
{
	NSEnumerator *keyEnum = [[_loadedModuleDictionary allKeys] objectEnumerator];
	NSString *nextKey;
	
	NSMutableArray *result = [NSMutableArray array];
	while (nextKey = [keyEnum nextObject]) {
		LVModule *module = [_loadedModuleDictionary objectForKey:nextKey];		
		
		NSString *moduleType = [NSString stringWithUTF8String:[module moduleObject]->moduleType()];
		NSString *localizedName = [NSString stringWithUTF8String:[module moduleObject]->localizedName(_loaderService->locale())];
		
		[result addObject:[NSDictionary dictionaryWithObjectsAndKeys:nextKey, LVDOModuleIdentifierKey, moduleType, LVDOModuleTypeKey, localizedName, LVDOModuleLocalizedNameKey, nil]];
	}
	
	return result;
}
@end

@implementation LVModuleManager (ProtectedMethods)
- (NSString *)userDataPathForModuleID:(NSString *)moduleID
{
    NSArray *dirs = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    NSAssert([dirs count], @"NSSearchPathForDirectoriesInDomains");	

	NSString *userPath = [[dirs objectAtIndex:0] stringByAppendingPathComponent:OPENVANILLA_NAME];

	BOOL isDir = YES;
	if (![[NSFileManager defaultManager] fileExistsAtPath:userPath isDirectory:&isDir]) {
		[[NSFileManager defaultManager] createDirectoryAtPath:userPath attributes:nil];
	}	
	NSAssert1(isDir, @"%@ must be a directory", userPath);	
	
	NSString *userDataPath = [userPath stringByAppendingPathComponent:@"UserData"];
	isDir = YES;
	if (![[NSFileManager defaultManager] fileExistsAtPath:userDataPath isDirectory:&isDir]) {
		[[NSFileManager defaultManager] createDirectoryAtPath:userDataPath attributes:nil];
	}	
	NSAssert1(isDir, @"%@ must be a directory", userDataPath);	
	
	NSString *moduleDataPath = [userDataPath stringByAppendingPathComponent:moduleID];
	isDir = YES;
	if (![[NSFileManager defaultManager] fileExistsAtPath:moduleDataPath isDirectory:&isDir]) {
		[[NSFileManager defaultManager] createDirectoryAtPath:moduleDataPath attributes:nil];
	}	
	NSAssert1(isDir, @"%@ must be a directory", moduleDataPath);		
	
    return [moduleDataPath stringByAppendingString:@"/"];
}
@end
