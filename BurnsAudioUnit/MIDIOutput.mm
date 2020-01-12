//
//  MIDIOutput.m
//  BurnsAudioUnit
//
//  Created by tom on 2020-01-08.
//  Copyright © 2020 tom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MIDIOutput.h"
#import <vector>

typedef struct {
    AUParameterAddress address;
    float value;
} midiOutputParameterState;

@implementation MIDIOutput {
    std::vector<midiOutputParameterState> outputParameterState;
}

- (id) initWithParameterTree:(AUParameterTree *) tree midiProcessor:(MIDIProcessor *) processor {
    self = [super init];
    
    _parameterTree = tree;
    outputParameterState.reserve(tree.allParameters.count);
    
    return self;
}

- (void) emitMIDIOutput:(AUMIDIOutputEventBlock *)midiOutput {
    
}


@end
