#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	int radius = 300;
	
	auto noise_seed = glm::vec2(ofRandom(1500), ofRandom(1500));

	for (int theta = 45; theta < 180; theta += 90) {

		for (int phi = 0; phi < 360; phi += 1) {

			auto index = this->face.getNumVertices();
			vector<glm::vec3> vertices;

			vertices.push_back(glm::vec3(
				radius * cos(phi * DEG_TO_RAD) * sin((theta + 1) * DEG_TO_RAD),
				radius * sin(phi * DEG_TO_RAD) * sin((theta + 1) * DEG_TO_RAD),
				radius * cos((theta + 1) * DEG_TO_RAD)
			));
			vertices.push_back(glm::vec3(
				radius * cos((phi + 1) * DEG_TO_RAD) * sin((theta + 1) * DEG_TO_RAD),
				radius * sin((phi + 1) * DEG_TO_RAD) * sin((theta + 1) * DEG_TO_RAD),
				radius * cos((theta + 1) * DEG_TO_RAD)
			));
			vertices.push_back(glm::vec3(
				radius * cos((phi + 1) * DEG_TO_RAD) * sin((theta - 1) * DEG_TO_RAD),
				radius * sin((phi + 1) * DEG_TO_RAD) * sin((theta - 1) * DEG_TO_RAD),
				radius * cos((theta - 1) * DEG_TO_RAD)
			));
			vertices.push_back(glm::vec3(
				radius * cos(phi * DEG_TO_RAD) * sin((theta - 1) * DEG_TO_RAD),
				radius * sin(phi * DEG_TO_RAD) * sin((theta - 1) * DEG_TO_RAD),
				radius * cos((theta - 1) * DEG_TO_RAD)
			));

			vertices.push_back(glm::vec3(
				(radius - 30) * cos(phi * DEG_TO_RAD) * sin((theta + 1) * DEG_TO_RAD),
				(radius - 30) * sin(phi * DEG_TO_RAD) * sin((theta + 1) * DEG_TO_RAD),
				radius * cos((theta + 1) * DEG_TO_RAD)
			));
			vertices.push_back(glm::vec3(
				(radius - 30) * cos((phi + 1) * DEG_TO_RAD) * sin((theta + 1) * DEG_TO_RAD),
				(radius - 30) * sin((phi + 1) * DEG_TO_RAD) * sin((theta + 1) * DEG_TO_RAD),
				radius * cos((theta + 1) * DEG_TO_RAD)
			));
			vertices.push_back(glm::vec3(
				(radius - 30) * cos((phi + 1) * DEG_TO_RAD) * sin((theta - 1) * DEG_TO_RAD),
				(radius - 30) * sin((phi + 1) * DEG_TO_RAD) * sin((theta - 1) * DEG_TO_RAD),
				radius * cos((theta - 1) * DEG_TO_RAD)
			));
			vertices.push_back(glm::vec3(
				(radius - 30) * cos(phi * DEG_TO_RAD) * sin((theta - 1) * DEG_TO_RAD),
				(radius - 30) * sin(phi * DEG_TO_RAD) * sin((theta - 1) * DEG_TO_RAD),
				radius * cos((theta - 1) * DEG_TO_RAD)
			));

			for (auto& vertex : vertices) {

				auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(glm::vec2(noise_seed.y, vertex.x * 0.002 + ofGetFrameNum() * 0.01)), 0, 1, -360, 360) * (float)DEG_TO_RAD, glm::vec3(1, 0, 0));
				vertex = glm::vec4(vertex, 0) * rotation_x;
			}

			this->face.addVertices(vertices);
			this->frame.addVertices(vertices);

			this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 2);
			this->face.addIndex(index + 0); this->face.addIndex(index + 2); this->face.addIndex(index + 3);

			this->face.addIndex(index + 4); this->face.addIndex(index + 5); this->face.addIndex(index + 6);
			this->face.addIndex(index + 4); this->face.addIndex(index + 6); this->face.addIndex(index + 7);

			this->face.addIndex(index + 0); this->face.addIndex(index + 4); this->face.addIndex(index + 5);
			this->face.addIndex(index + 0); this->face.addIndex(index + 5); this->face.addIndex(index + 1);

			this->face.addIndex(index + 2); this->face.addIndex(index + 6); this->face.addIndex(index + 7);
			this->face.addIndex(index + 2); this->face.addIndex(index + 7); this->face.addIndex(index + 3);

			this->frame.addIndex(index + 0); this->frame.addIndex(index + 1);
			this->frame.addIndex(index + 2); this->frame.addIndex(index + 3);

			this->frame.addIndex(index + 4); this->frame.addIndex(index + 5);
			this->frame.addIndex(index + 6); this->frame.addIndex(index + 7);

			ofColor color;
			theta == 45 ? color.setHsb(ofMap(phi, 0, 360, 0, 255), 255, 255) : color.setHsb(ofMap(phi, 0, 360, 255, 0), 255, 255);
			for (int i = 0; i < vertices.size(); i++) {

				this->face.addColor(color);
				this->frame.addColor(color);
			}
		}
	}
}


//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateZ(90);

	this->face.draw();
	this->frame.drawWireframe();

	this->cam.end();

	int start = 280;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}