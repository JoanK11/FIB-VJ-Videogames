#include "IntBox.h"
IntBox::IntBox(glm::vec2 pos, glm::vec2 size, ShaderProgram* p) {
	isWall = true;
	text = new Texture();
	text->loadFromFile("images/interrogante.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spr = Sprite::createSprite(glm::ivec2(size.x, size.y), glm::vec2(0.25, 1.0), text, p);
	
	spr->setNumberAnimations(2);
	spr->setAnimationSpeed(0, 6);
	spr->addKeyframe(0, glm::vec2(0., 0.));
	spr->addKeyframe(0, glm::vec2(0.25, 0.));
	spr->addKeyframe(0, glm::vec2(0.50, 0.));
	spr->setAnimationSpeed(1, 20);
	spr->addKeyframe(1, glm::vec2(0.75, 0.));

	spr->setPosition(pos);
	spr->changeAnimation(0);
}
void IntBox::update(float dt) {
	spr->update((int)dt);
}
void IntBox::render(){
	spr->render();
}