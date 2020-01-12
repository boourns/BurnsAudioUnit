//
//  MIDIOutput.h
//  BurnsAudioUnit
//
//  Created by tom on 2020-01-08.
//  Copyright © 2020 tom. All rights reserved.
//

#ifndef MIDIOutput_h
#define MIDIOutput_h

#import <AVFoundation/AVFoundation.h>
#import "MIDIProcessor.hpp"

@interface MIDIOutput : NSObject

@property AUParameterTree *parameterTree;
@property MIDIProcessor *midiProcessor;

- (id) initWithParameterTree:(AUParameterTree *) tree midiProcessor:(MIDIProcessor *) processor;
- (void) emitMIDIOutput:(AUMIDIOutputEventBlock *)midiOutput;

@end

#endif /* MIDIOutput_h */
