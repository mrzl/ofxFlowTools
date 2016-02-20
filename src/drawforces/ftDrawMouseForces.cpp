//
//  ftDrawForces.cpp
//  ofxFlowToolsExample
//
//  Created by Ties East on 18/6/15.
//
//

#include "ftDrawMouseForces.h"


namespace flowTools {

	ftDrawMouseForces::ftDrawMouseForces() {
		ofAddListener( ofEvents().mouseMoved, this, &ftDrawMouseForces::mouseMoved );
		ofAddListener( ofEvents().mouseDragged, this, &ftDrawMouseForces::mouseDragged );
	}

	ftDrawMouseForces::~ftDrawMouseForces() {
		ofRemoveListener( ofEvents().mouseMoved, this, &ftDrawMouseForces::mouseMoved );
		ofRemoveListener( ofEvents().mouseDragged, this, &ftDrawMouseForces::mouseDragged );
	}

	void ftDrawMouseForces::setup( int _simulationWidth, int _simulationHeight, int _densityWidth, int _densityHeight ) {
		simulationWidth = _simulationWidth;
		simulationHeight = _simulationHeight;
		densityWidth = ( !_densityWidth ) ? simulationWidth : _densityWidth;
		densityHeight = ( !_densityHeight ) ? simulationHeight : _densityHeight;
			
		numDrawForces = 8;
		drawForces = new ftDrawForce[ numDrawForces ];
		drawForces[ 0 ].setup( densityWidth, densityHeight, FT_DENSITY, true );
		drawForces[ 0 ].setName( "draw full res" );
		drawForces[ 1 ].setup( simulationWidth, simulationHeight, FT_VELOCITY, true );
		drawForces[ 1 ].setName( "draw flow res 1" );
		drawForces[ 2 ].setup( simulationWidth, simulationHeight, FT_TEMPERATURE, true );
		drawForces[ 2 ].setName( "draw flow res 2" );
		drawForces[ 3 ].setup( simulationWidth, simulationHeight, FT_OBSTACLE, true );
		drawForces[ 3 ].setName( "draw flow res 3" );
		drawForces[ 4 ].setup( densityWidth, densityHeight, FT_DENSITY, false );
		drawForces[ 4 ].setName( "draw full res" );
		drawForces[ 5 ].setup( simulationWidth, simulationHeight, FT_VELOCITY, false );
		drawForces[ 5 ].setName( "draw flow res 1" );
		drawForces[ 6 ].setup( simulationWidth, simulationHeight, FT_TEMPERATURE, false );
		drawForces[ 6 ].setName( "draw flow res 2" );
		drawForces[ 7 ].setup( simulationWidth, simulationHeight, FT_OBSTACLE, false );
		drawForces[ 7 ].setName( "draw flow res 3" );

		leftButtonParameters.setName( "mouse left button" );
		leftButtonParameters.add( doResetDrawForces.set( "reset", false ) );
		rightButtonParameters.setName( "mouse right button" );
		rightButtonParameters.add( doResetDrawForces.set( "reset", false ) );
		doResetDrawForces.addListener( this, &ftDrawMouseForces::resetDrawForcesListner );
		for( int i = 0; i < numDrawForces / 2; i++ ) {
			leftButtonParameters.add( drawForces[ i ].parameters );
			rightButtonParameters.add( drawForces[ i + ( numDrawForces / 2 ) ].parameters );
		}
	}

	void ftDrawMouseForces::update( float _deltaTime ) {
		deltaTime = _deltaTime;

		for( int i = 0; i < numDrawForces; i++ ) {
			drawForces[ i ].update();
		}
	}

	//--------------------------------------------------------------
	bool ftDrawMouseForces::didChange( int _index ) {
		if( _index < 0 || _index >= numDrawForces ) {
			ofLogWarning( "ftDrawMouseForces::getDrawForceType: index out of range" );
			return false;
		}
		else
			return drawForces[ _index ].didChange();
	}

	//--------------------------------------------------------------
	ftDrawForceType ftDrawMouseForces::getType( int _index ) {
		if( _index < 0 || _index >= numDrawForces ) {
			ofLogWarning( "ftDrawMouseForces::getDrawForceType: index out of range" );
			return FT_NONE;
		}
		else
			return drawForces[ _index ].getType();
	}

	void ftDrawMouseForces::setType( int _index, ftDrawForceType _type )
	{
		if( _index < 0 || _index >= numDrawForces ) {
			ofLogWarning( "ftDrawMouseForces::setType: index out of range" );
		}
		else {
			drawForces[ _index ].setType( _type );
		}
	}

	//--------------------------------------------------------------
	ofTexture& ftDrawMouseForces::getTextureReference( int _index ) {
		if( _index < 0 || _index >= numDrawForces ) {
			ofLogError( "ftDrawMouseForces::getTexture: index out of range" );
		}
		else
			return drawForces[ _index ].getTexture();

	}

	//--------------------------------------------------------------
	float ftDrawMouseForces::getStrength( int _index ) {
		if( _index < 0 || _index >= numDrawForces ) {
			ofLogWarning( "ftDrawMouseForces::getStrength: index out of range" );
			return 0;
		}
		else {
			if( drawForces[ _index ].getIsTemporary() ) {
				return drawForces[ _index ].getStrength();
			}
			else {
				return drawForces[ _index ].getStrength() * deltaTime;;
			}
		}
	}

	void ftDrawMouseForces::setStrength( int _index, float _strength )
	{
		drawForces[ _index ].setStrength( _strength );
	}

	void ftDrawMouseForces::setStrength( float _strength )
	{
		for( int i = 0; i < numDrawForces; i++ ) {
			drawForces[ i ].setStrength( _strength );
		}
	}

	//--------------------------------------------------------------
	void ftDrawMouseForces::mouseDragged( ofMouseEventArgs& mouse ) {
		ofVec2f normalizedMouse;

		normalizedMouse.set( mouse.x / ( float ) ofGetWindowWidth(), mouse.y / ( float ) ofGetWindowHeight() );

		ofVec2f velocity = normalizedMouse - lastNormalizedMouse;

		if( mouse.button == 0 ) {

			for( int i = 0; i < numDrawForces / 2 - 1; i++ ) {
				if( drawForces[ i ].getType() == FT_VELOCITY )
					drawForces[ i ].setForce( velocity );
				drawForces[ i ].applyForce( normalizedMouse );
			}
		}
		else {

			for( int i = numDrawForces / 2; i < numDrawForces - 1; i++ ) {
				if( drawForces[ i ].getType() == FT_VELOCITY )
					drawForces[ i ].setForce( velocity );
				drawForces[ i ].applyForce( normalizedMouse );
			}
		}
		lastNormalizedMouse.set( normalizedMouse );
	}

	//--------------------------------------------------------------
	void ftDrawMouseForces::mouseMoved( ofMouseEventArgs& mouse ) {
		ofVec2f normalizedMouse;
		normalizedMouse.set( mouse.x / ( float ) ofGetWindowWidth(), mouse.y / ( float ) ofGetWindowHeight() );
		lastNormalizedMouse.set( normalizedMouse );
	}

	flowTools::ftDrawForce ftDrawMouseForces::getTemporaryDrawForce( int index )
	{
		return drawForces[ index ];
	}

	flowTools::ftDrawForce ftDrawMouseForces::getPermanentDrawForce( int index )
	{
		return drawForces[ numDrawForces / 2 + index ];
	}
}