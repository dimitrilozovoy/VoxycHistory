//
//  IOSAudio.h
//  Fateless
//
//  Created by Dimitri Lozovoy on 12/3/18.
//
//

#ifndef IOSAudio_h
#define IOSAudio_h

#import <Foundation/Foundation.h>
#import <string>

@interface IOSAudio: NSObject

- (int) loadSound:(std::string) fname;
- (int) playSound:(int) sndID;
- (int) stopSound:(int) sndID;

@end

extern IOSAudio *g_iosAudio;

#endif /* IOSAudio_h */
