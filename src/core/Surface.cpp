//
//  Surface.cpp
//  PixEngine
//
//  Created by rodo on 17/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "Surface.hpp"
#include "Utils.hpp"
#include "Font.hpp"
#include "Canvas2D.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
#pragma clang diagnostic ignored "-Wunused-parameter"
namespace Pix {

	std::string Surface::TAG = "Surface";

	constexpr float Surface::VERTICES[32];
	constexpr unsigned int Surface::INDICES[6];

	Surface::Surface(int width, int height, const std::string &shaderName, const std::string &samplerName, bool blend)
			: nWidth(width),
			  nHeight(height),
			  pShader(new Shader(shaderName)),
			  sSamplerName(samplerName),
			  bBlend(blend){

		if (DBG) LogV(TAG, SF("Creating, %dx%d, shader %s", width, height, shaderName.c_str()));
		pActiveTexture = new Texture2D(width, height);
		pCanvas = new Canvas2D(pActiveTexture->buffer(), new Font());
	}

	Surface::~Surface() {
		delete pActiveTexture;
		delete pShader;
		pShader = nullptr;
		pActiveTexture = nullptr;
	}

	bool Surface::init(Fu *engine) {

		if (pActiveTexture == nullptr) return false;

		LayerVao::add((float *) VERTICES, sizeof(VERTICES),
					  (unsigned *) INDICES, sizeof(INDICES));

		pActiveTexture->upload();

		pShader->use();
		pShader->textureUnit(sSamplerName, pActiveTexture);
		pShader->setVec2("iResolution", nWidth, nHeight);
		pShader->stop();

		return true;

	}

	void Surface::tick(Fu *engine, float fElapsedTime) {

		// only update texture if it has changed
		if (buffer()->clearDirty()) pActiveTexture->update();

		pShader->use();

		if (bBlend) {
			// blend the surface with back layers
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		}
		
		draw();
		
		if (bBlend) {
			glDisable(GL_BLEND);
		}
		
		pShader->stop();

	}

}


#pragma clang diagnostic pop
