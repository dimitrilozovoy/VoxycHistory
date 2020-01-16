//
//  IOSAudio.m
//  Fateless
//
//  Created by Dimitri Lozovoy on 12/3/18.
//
//

#import <Foundation/Foundation.h>
#import "IOSAudio.h"
#import "DDLUtils.hpp"
#import <AVFoundation/AVAudioPlayer.h>
#import <vector>

IOSAudio *g_iosAudio;

@implementation IOSAudio

std::vector<AVAudioPlayer *> players;

- (int) loadSound:(std::string) fname
{
    std::string stdfullfname = GetFullFilename(fname);
    NSString *fullfname = [NSString stringWithFormat:@"%s", stdfullfname.c_str()];
     
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString *path = [NSString stringWithFormat:fullfname, resourcePath];
     
    NSURL *url = [NSURL fileURLWithPath:path];
    NSError *error = nil;
     
    AVAudioPlayer *audioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url
    error:&error];

    if (audioPlayer != nil)
    {
        players.push_back(audioPlayer);
        return players.size() - 1;
    }
    else
        return -1;
}

- (int) playSound:(int) sndID
{
    if (sndID == -1)
        return -1;
    
    AVAudioPlayer *audioPlayer = players[sndID];
    
    if (audioPlayer != nullptr)
        [audioPlayer play];
    
    return 0;
}

- (int) stopSound:(int) sndID
{
    if (sndID == -1)
        return -1;
    
    AVAudioPlayer *audioPlayer = players[sndID];
    
    if (audioPlayer != nullptr)
        [audioPlayer stop];
    
    return 0;
}

@end
