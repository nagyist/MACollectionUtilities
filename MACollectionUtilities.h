//
//  MACollectionUtilities.h
//  MACollectionUtilities
//
//  Created by Michael Ash on 10/11/10.
//  Copyright 2010 Michael Ash. All rights reserved.
//

#import <Foundation/Foundation.h>


#define ARRAY(...) ([NSArray arrayWithObjects: IDARRAY(__VA_ARGS__) count: IDCOUNT(__VA_ARGS__)])
#define SET(...) ([NSSet setWithObjects: IDARRAY(__VA_ARGS__) count: IDCOUNT(__VA_ARGS__)])

// this is key/object order, not object/key order, thus all the fuss
#define DICT(...) MADictionaryWithKeysAndObjects(IDARRAY(__VA_ARGS__), IDCOUNT(__VA_ARGS__) / 2)

#define MAP(collection, ...) EACH_WRAPPER([collection ma_map: ^id (id obj) { return (__VA_ARGS__); }])
#define SELECT(collection, ...) EACH_WRAPPER([collection ma_select: ^BOOL (id obj) { return (__VA_ARGS__) != 0; }])
#define REJECT(collection, ...) EACH_WRAPPER([collection ma_select: ^BOOL (id obj) { return (__VA_ARGS__) == 0; }])
#define MATCH(collection, ...) EACH_WRAPPER([collection ma_match: ^BOOL (id obj) { return (__VA_ARGS__) != 0; }])

#define EACH(array) MAEachHelper(array, &MA_eachTable)

@interface NSArray (MACollectionUtilities)

- (NSArray *)ma_map: (id (^)(id obj))block;
- (NSArray *)ma_select: (BOOL (^)(id obj))block;
- (id)ma_match: (BOOL (^)(id obj))block;

@end


// ===========================================================================
// internal utility whatnot that needs to be externally visible for the macros
#define IDARRAY(...) ((id[]){ __VA_ARGS__ })
#define IDCOUNT(...) (sizeof(IDARRAY(__VA_ARGS__)) / sizeof(id))
#define EACH_WRAPPER(...) (^{ __block NSMapTable *MA_eachTable = nil; (void)MA_eachTable; return __VA_ARGS__; }())

static inline NSDictionary *MADictionaryWithKeysAndObjects(id *keysAndObjs, NSUInteger count)
{
    id keys[count];
    id objs[count];
    for(NSUInteger i = 0; i < count; i++)
    {
        keys[i] = keysAndObjs[i * 2];
        objs[i] = keysAndObjs[i * 2 + 1];
    }
    
    return [NSDictionary dictionaryWithObjects: objs forKeys: keys count: count];
}

static inline id MAEachHelper(NSArray *array, NSMapTable **eachTablePtr)
{
    if(!*eachTablePtr)
        *eachTablePtr = [NSMapTable mapTableWithStrongToStrongObjects];
    NSEnumerator *enumerator = [*eachTablePtr objectForKey: array];
    if(!enumerator)
    {
        enumerator = [array objectEnumerator];
        [*eachTablePtr setObject: enumerator forKey: array];
    }
    return [enumerator nextObject];
}
