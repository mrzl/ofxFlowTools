#pragma once

#include "ofMain.h"
#include "ftFbo.h"
#include "ftDrawForce.h"

namespace flowTools {

	class ftDrawMouseForces {

	public:
		ftDrawMouseForces();
		~ftDrawMouseForces();

		void				setup( int _simulationWidth, int _simulationHeight, int _densityWidth = 0, int _densityHeight = 0 );
		void				update( float _deltaTime );
		void				reset() { for( int i = 0; i < numDrawForces; i++ ) drawForces[ i ].reset(); }

		int					getNumForces() { return numDrawForces; }

		bool				didChange( int _index );
		ftDrawForceType		getType( int _index );
		void				setType( int _index, ftDrawForceType _type );
		ofTexture&			getTextureReference( int _index );
		float				getStrength( int _index );

		//		ofParameterGroup	parameters;
		ofParameterGroup	leftButtonParameters;
		ofParameterGroup	rightButtonParameters;

		void				mouseMoved( ofMouseEventArgs & mouse );
		void				mouseDragged( ofMouseEventArgs & mouse );
		void				permanentImpulse( int x, int y, float direction, bool isObstacle );

	private:
		ofParameter<bool>	doResetDrawForces;
		void				resetDrawForcesListner( bool& _value ) { if( _value ) { reset(); }; doResetDrawForces.set( false ); }

		int					numDrawForces;
		ftDrawForce*		drawForces;

		float				deltaTime;

		int					simulationWidth;
		int					simulationHeight;
		int					densityWidth;
		int					densityHeight;

		ofVec2f				lastNormalizedMouse;

	};
}