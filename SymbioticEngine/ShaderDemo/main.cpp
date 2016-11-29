#include <stdio.h>
#include <SBE/FpsLimiter.h>
#include <SBE/Window.h>
#include <SBE/ShaderProgram.h>
#include <SBE/SpriteBatch.h>
#include <SBE/ResourceManager.h>

enum class State { RUNNING, EXIT };

void configureShaders();

State programState = State::RUNNING;
sbe::FpsLimiter fpsHandle;
sbe::Window windowHandle;
sbe::ShaderProgram shaderHandle;
sbe::SpriteBatch spriteHandle;
sbe::ResourceManager resourceHandle;
sbe::GLTexture mainTexture;

int main(int argc, char *argv[]) {
	fpsHandle.setMaxFPS(10000.0f);
	windowHandle.create("Shader Demo", 1024, 768, 0);
	configureShaders();
	spriteHandle.initialize();
	mainTexture = resourceHandle.getTexture("background.png");
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClearDepth(1.0f);
	
	while (programState == State::RUNNING) {
		fpsHandle.start();

		SDL_Event evnt;
		while (SDL_PollEvent(&evnt)) {
			switch (evnt.type) {
			case SDL_QUIT:
				programState = State::EXIT;
				break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Bind texture
		glActiveTexture(GL_TEXTURE0);
		GLint textureLocation = shaderHandle.getUniformLocation("mySampler");
		glUniform1i(textureLocation, 0);

		shaderHandle.startUsing();

		spriteHandle.startUsing();

		glm::vec4 destRect(0.0f, 0.0f, 1.0f, 1.0f);
		spriteHandle.draw(destRect, mainTexture.id);
		spriteHandle.renderBatch();
		spriteHandle.stopUsing();

		glBindTexture(GL_TEXTURE_2D, 0);

		shaderHandle.stopUsing();

		windowHandle.swapBuffer();

		static int frameCounter;
		if (frameCounter++ == 20) {
			printf("FPS: %d\n", (int)fpsHandle.stop());
			frameCounter = 0;
		}
	}
	return 0;
}

void configureShaders() {
	shaderHandle.compileShaders("vs.txt", "fs.txt");
	shaderHandle.addAttribute("vertexPosition");
	shaderHandle.addAttribute("vertexColour");
	shaderHandle.addAttribute("vertexUV");
	shaderHandle.linkShaders();
}