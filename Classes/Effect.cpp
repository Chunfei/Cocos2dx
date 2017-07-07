#include "Effect.h"

USING_NS_CC;


bool Effect::initGLProgramState(const std::string &fragmentFilename)
{
	auto fileUtiles = FileUtils::getInstance();
	auto fragmentFullPath = fileUtiles->fullPathForFilename(fragmentFilename);
	auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
	auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	_fragSource = fragSource;
#endif

	_glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	_glprogramstate->retain();

	return _glprogramstate != nullptr;
}

Effect::Effect()
	: _glprogramstate(nullptr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	_backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
		[this](EventCustom*)
	{
		auto glProgram = _glprogramstate->getGLProgram();
		glProgram->reset();
		glProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, _fragSource.c_str());
		glProgram->link();
		glProgram->updateUniforms();
	}
	);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif
}

Effect::~Effect()
{
	CC_SAFE_RELEASE_NULL(_glprogramstate);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	Director::getInstance()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}

