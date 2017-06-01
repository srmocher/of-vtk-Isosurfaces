#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include <vtkImageData.h>
#include <vtkImageReader.h>
#include <vtkSmartPointer.h>
#include "marchingcubes.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
private:
	ofVboMesh mesh;
	ofxPanel gui;
	ofEasyCam camera;
	string filePath;
	bool wireFrame,computeNormals,drawBoundingBox;
	float extrusionAmount;
	ofxFloatSlider *isoSlider;
	ofxToggle *wireframeToggler,*normalsToggler,*boundingBoxToggler,*lightingToggler;
	ofxIntSlider *wireFrameOpacitySlider, *surfaceOpacitySlider;
	float ***data;
	ofVec3f boundingBoxPosition,boundingBoxSize;
	int *dims;
	double minVal, maxVal;
	int wireFrameOpacity, surfaceOpacity;
	float currentIsoValue;
	ofxLabel trianglesCount,selectedDataset;
	ofxColorSlider wireFrameColorSlider, meshColorSlider;
	vtkSmartPointer<vtkImageData> image;
	MarchingCubes *mc;
	ofxButton fileLoadButton;
	ofLight pointLight;
	bool lightingEnabled;
	void readVtiFile();
	void polygonize();
	void isoValueChanged(float &val);
	void wireFrameToggled(bool &val);
	void normalsToggled(bool &val);
	void boxToggled(bool &val);
	void createMesh();
	void fileSelected();
	void wireFrameOpacityUpdated(int &);
	void surfaceOpacityUpdated(int &);
	void lightingToggled(bool &val);
};
