#pragma once

#include "ofMain.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class WaveSolver {
public:
	WaveSolver() {
		_u0.resize(kWaveGrid * kWaveGrid);
		_u1.resize(kWaveGrid * kWaveGrid);
		_uminus1.resize(kWaveGrid * kWaveGrid);
		this->reset();
		
		float lambda = _params.lambda();
		printf("lambda = %f\n", lambda);
	}
	void reset() {
		auto gauss = [](float x, float sigma) {
			return 1.0f / glm::sqrt(glm::two_pi<float>()) * sigma * glm::exp(-x * x / (2.0f * sigma * sigma));
		};

		int cx1 = kWaveGrid / 3;
		int cy1 = kWaveGrid / 3;
		int cx2 = kWaveGrid * 2 / 3;
		int cy2 = kWaveGrid * 2 / 3;
		for (int x = 1; x < kWaveGrid - 1; ++x) {
			for (int y = 1; y < kWaveGrid - 1; ++y) {
				int index = this->valueIndex(x, y);
				float value = 0.0f;

				{
					float norm = glm::distance(glm::vec2(x, y), glm::vec2(cx1, cy1));
					value += gauss(norm, 3.0f) * 20.0f;
				}
				{
					float norm = glm::distance(glm::vec2(x, y), glm::vec2(cx2, cy2));
					value += gauss(norm, 3.0f) * 20.0f;
				}
				
				_u0[index] = value;
			}
		}

		_uminus1 = _u1 = _u0;
	}
	void step() {
		// float k = 0.01f;

		for (int x = 1; x < kWaveGrid - 1; ++x) {
			for (int y = 1; y < kWaveGrid - 1; ++y) {
				int index = this->valueIndex(x, y);

				float uL = _u0[this->valueIndex(x - 1, y)];
				float uR = _u0[this->valueIndex(x + 1, y)];
				float uT = _u0[this->valueIndex(x, y - 1)];
				float uB = _u0[this->valueIndex(x, y + 1)];

				float uminus1 = _uminus1[index];
				float u0 = _u0[index];

				float g = u0 - uminus1;
				
				// 減衰項
				// g = g - g * k;

				float lambda = _params.lambda();
				float u1 = u0 + g + lambda * lambda * 0.5f * (uL - 2.0f * u0 + uR) + lambda * lambda * 0.5f * (uT - 2.0f * u0 + uB);

				_u1[index] = u1;
			}
		}

		std::swap(_uminus1, _u0);
		std::swap(_u0, _u1);
	}

	enum {
		kWaveGrid = 300
	};
	struct P {
		// 空間分割数
		const float N = kWaveGrid;

		// 微小時間
		float tao = 1.0f / 60.0f;

		// 辺の長さ
		float I = 100.0f;

		// 波動方程式の定数（移動速度？
		float c = 10.0f;

		// 微小幅x
		float h() const {
			return I / N;
		}

		// これは1より小さい必要があるらしい。が、いったん気にしない
		float lambda() const {
			return c * tao / h();
		}

		// 総合時間は考えないのでこれはなし
		// float T;
		// float M() const {
		// 	return T / tao;
		// }
	};
	
	int valueIndex(int x, int y) const {
		return y * kWaveGrid + x;
	}
	float value(int x, int y) const {
		return _u0[valueIndex(x, y)];
	}
	int widthN() const {
		return kWaveGrid;
	}
	int heightN() const {
		return kWaveGrid;
	}

	float width() const {
		return _params.I;
	}
	float height() const {
		return _params.I;
	}
private:
	std::vector<float> _uminus1;
	std::vector<float> _u0;
	std::vector<float> _u1;

	P _params;
};

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofEasyCam _camera;
	WaveSolver _wave;
	ofMesh _mesh;

};
