/***************************************************************************
*
*  testApp.cpp
* 
*  Author: Jimbo Zhang <dr.jimbozhang@gmail.com>
*  Copyright 2010 Jimbo Zhang. All rights reserved.       
*
*
* This file is part of ofxASR, an openframeworks addon for speech
* recognition.
*
* ofxASR is free software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ofxASR is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with ofxASR. If not, see <http://www.gnu.org/licenses/>.
*
*
***************************************************************************/

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    /* 
     * ofxASR supports two modes:
     * Mode-4: Any words in the dictionary can be recognized. The 
     *         speaker is allowed to speak anything. In this mode,
     *         it is free to speak but not very accuracy.
     * Mode-2: A list contains some sentences was required and the
     *         speaker must select one sentence to read. If she/he
     *         spoke something not in the list, the engine will 
     *         return the most similar sentence from the list. This
     *         mode seems not very "free" but it is accuracy.
     * The usages of the both two modes are showed below, however, 
     * you only need choose one mode to use.
     */

    //////////////////////////////////////////////////////////////////////////
    // Create a Mode-4 engine
    //////////////////////////////////////////////////////////////////////////
    engine1_listenFromList = new ofxSphinxASR;
    e = new ofAsrEngineArgs;

    // set sample rate to 16000 Hz
    e->samplerate = 16000;

    // set mode code
    e->sphinx_mode = 4;

    // set the folder of the acoustics model
    e->sphinxmodel_am = "sphinxmodel/digit.8gau";

    // set the path of the language model
    e->sphinxmodel_lm = "sphinxmodel/digit.lm.DMP";

    // set the path of the dict and the filler dict
    e->sphinxmodel_dict = "sphinxmodel/digit.dict";
    e->sphinxmodel_fdict = "sphinxmodel/digit.fdict";

    // Initial the engine
    int retval = engine1_listenFromList->engineInit(e);
    if (retval != OFXASR_SUCCESS) {
        printf("ASR Engine initial failed. Error Code: %d\n", retval);
    }

    //////////////////////////////////////////////////////////////////////////
    // Create a Mode-2 engine
    //////////////////////////////////////////////////////////////////////////
    engine2_listenFromAnything = new ofxSphinxASR;
    e->sphinx_mode = 2;

    // Mode-2 need a list. Add sentences to the list
    e->sphinx_candidate_sentences.push_back("how are you");
    e->sphinx_candidate_sentences.push_back("good morning");
    e->sphinx_candidate_sentences.push_back("are you ok");
    e->sphinx_candidate_sentences.push_back("thank you");
    
    retval = engine2_listenFromAnything->engineInit(e);
    if (retval != OFXASR_SUCCESS) {
        printf("ASR Engine initial failed. Error Code: %d\n", retval);
    }
}

//--------------------------------------------------------------
void testApp::exit(){
    if (engine1_listenFromList != NULL) {
        engine1_listenFromList->engineExit();
        delete engine1_listenFromList;
        engine1_listenFromList = NULL;
    }
    if (engine2_listenFromAnything != NULL) {
        engine2_listenFromAnything->engineExit();
        delete engine2_listenFromAnything;
        engine2_listenFromAnything = NULL;
    }
    if (e != NULL) {
        delete e;
        e = NULL;
    }
}

//--------------------------------------------------------------
void testApp::mousePressed( int x, int y, int button )
{
    // Open the engines.
    // Once the engine be opened, it will start to recognize the sound from ofSoundStream
    int retval = engine1_listenFromList->engineOpen();
    if (retval != OFXASR_SUCCESS) {
        printf("ASR Engine failed to open. Error Code: %d\n", retval);
    }
    retval = engine2_listenFromAnything->engineOpen();
    if (retval != OFXASR_SUCCESS) {
        printf("ASR Engine failed to open. Error Code: %d\n", retval);
    }
    
    // Start to record. Note that Mac OS do not support 16k sample rate
    ofSoundStreamSetup(0, 1, this, 16000, 256, 2);
}

//--------------------------------------------------------------
void testApp::mouseReleased()
{
    // Close the engines
    int retval = engine1_listenFromList->engineClose();
    if (retval != OFXASR_SUCCESS) {
        printf("ASR Engine failed to close. Error Code: %d\n", retval);
    }
    retval = engine2_listenFromAnything->engineClose();
    if (retval != OFXASR_SUCCESS) {
        printf("ASR Engine failed to close. Error Code: %d\n", retval);
    }
    
    ofSoundStreamClose();

    string s1(engine1_listenFromList->engineGetText());
    result_from_engine1 = s1;
    string s2(engine2_listenFromAnything->engineGetText());
    result_from_engine1 = s2;
};