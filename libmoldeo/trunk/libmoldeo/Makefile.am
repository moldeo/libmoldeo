ACLOCAL_AMFLAGS=-I m4

if DEBUG
DFLAGS = -g3 -O0 -fpermissive
else
DFLAGS = -O2 -fpermissive
endif

datadir = ${prefix}/share/moldeo
modulesdir = ${libdir}/moldeo
docdir = ${prefix}/share/doc/moldeo

AM_CPPFLAGS = ${DFLAGS} $(all_includes) -DDATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\"
AM_CXXFLAGS = ${DFLAGS} $(all_includes) -DDATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\"

lib_LTLIBRARIES = libmoldeo.la
libmoldeo_la_SOURCES = mo3dModelManager.cpp \
	moText.cpp \
	moAbstract.cpp \
	moActions.cpp \
	moArcBall.cpp \
	moBuckets.cpp \
	moConfig.cpp \
	moConnectors.cpp \
	moConsole.cpp \
	moConsoleState.cpp \
	moDataManager.cpp \
	moDebugManager.cpp \
	moDecoderManager.cpp \
	moDeviceCode.cpp \
	moDsGraph.cpp \
	moEffect.cpp \
	moEffectManager.cpp \
	moEffectState.cpp \
	moEventList.cpp \
	moFBManager.cpp \
	moFBO.cpp \
	moFileManager.cpp \
	moFontManager.cpp \
	moGLManager.cpp \
	moGsGraph.cpp \
	moGUIManager.cpp \
	moIODeviceManager.cpp \
	moIODevicePlugin.cpp \
	moLock.cpp \
	moLuaBase.cpp \
	moLunaClasses.cpp \
	moMasterEffect.cpp \
	moMasterPlugin.cpp \
	moMath.cpp \
	moMathCurve.cpp \
	moMathDMatrix.cpp \
	moMathDVector.cpp \
	moMathFFT.cpp \
	moMathFunction.cpp \
	moMathFunctionIndex.cpp \
	moMathManager.cpp \
	moMathNumericalAnalysis.cpp \
	moMathPolynomial.cpp \
	moMathVector3.cpp \
	moMathVector4.cpp \
	moMathVector.cpp \
	moMoldeoObject.cpp \
	moMotion.cpp \
	moNetManager.cpp \
	moOGLFT.cpp \
	moP5.cpp \
	moParam.cpp \
	moPlugin.cpp \
	moPostEffect.cpp \
	moPostPlugin.cpp \
	moPreConfig.cpp \
	moPreEffect.cpp \
	moPrePlugin.cpp \
	moPresets.cpp \
	moRenderManager.cpp \
	moResourceManager.cpp \
	moResourcePlugin.cpp \
	moSceneEffect.cpp \
	moScript.cpp \
	moScriptManager.cpp \
	moSequenceEffect.cpp \
	moShaderCG.cpp \
	moShader.cpp \
	moShaderGLSL.cpp \
	moShaderManager.cpp \
	moSoundManager.cpp \
	moTempo.cpp \
	moTextFilterParam.cpp \
	moTexture.cpp \
	moTextureFilter.cpp \
	moTextureFilterIndex.cpp \
	moTextureIndex.cpp \
	moTextureManager.cpp \
	moThread.cpp \
	moTimer.cpp \
	moTimeManager.cpp \
	moTypes.cpp \
	moValue.cpp \
	moVdpauGraph.cpp \
	moVideoGraph.cpp \
	moVideoManager.cpp \
	moXmlConfig.cpp \
	mo3ds.cpp \
	moFilterManager.cpp 

#	moMathMatrix.cpp \
#	moMathQuaternion.cpp 

libmoldeo_la_CPPFLAGS = ${FT2_CFLAGS} \
	${GST_CFLAGS} \
	${GTK2_CFLAGS} \
	${MUPARSER_CFLAGS} \
	${BOOST_CPPFLAGS} \
	${SDL_CFLAGS} \
	${LUA51_CFLAGS} 

libmoldeo_la_LIBADD = -ldl\
	${FT2_LIBS} \
	${GTK2_LIBS} \
	${SDL_LIBS} \
	${TINYXML_LIBS} \
	${GL_LIBS} \
	${GLE_LIBS} \
	${GLEW_LIBS} \
	${FTGL_LIBS} \
	${GLUT_LIBS} \
	${GLU_LIBS} \
	${FREEALUT_LIBS} \
	${FREEIMAGE_LIBS} \
	${MUPARSER_LIBS} \
	${BOOST_ASIO_LIB} \
	${BOOST_THREAD_LIB} \
	${BOOST_FILESYSTEM_LIB} \
	${BOOST_SYSTEM_LIB} \
	${LUA51_LIBS} \
	${GST_LIBS} 
#\
#	${CG_LIBS} \
#	${CGGL_LIBS} \
#	${OPENAL_LIBS} \
	#${TUIO_LIBS}

libmoldeo_includedir = $(includedir)/libmoldeo

libmoldeo_include_HEADERS = \
	moldeo.h \
	mo3dModelManager.h \
	mo3ds.h \
	moAbstract.h \
	moActions.h \
	moArcBall.h \
	moArray.h \
	moArrayH.h \
	moBasePlugin.h \
	moBuckets.h \
	moConfig.h \
	moConnectors.h \
	moConsole.h \
	moConsoleState.h \
	moDataManager.h \
	moDebugManager.h \
	moDecoderManager.h \
	moDeviceCode.h \
	moDsGraph.h \
	moEffect.h \
	moEffectManager.h \
	moEffectState.h \
	moEffectTemplate.h \
	moEventList.h \
	moFBManager.h \
	moFBO.h \
	moFileManager.h \
	moFileRead.h \
	moFileWrite.h \
	moFilterManager.h \
	moFontManager.h \
	moGLManager.h \
	moGsGraph.h \
	moGUIManager.h \
	moIODevice.h \
	moIODeviceManager.h \
	moIODevicePlugin.h \
	moLock.h \
	moLuaBase.h \
	moLunaClasses.h \
	moLuna.h \
	moMasterEffect.h \
	moMasterPlugin.h \
	moMathCurve.h \
	moMathDMatrix.h \
	moMathDVector.h \
	moMathFFT.h \
	moMathFunction.h \
	moMathFunctionIndex.h \
	moMath.h \
	moMathManager.h \
	moMathMatrix.h \
	moMathNumericalAnalysis.h \
	moMathPolynomial.h \
	moMathQuaternion.h \
	moMathVector3.h \
	moMathVector4.h \
	moMathVector.h \
	moMoldeoObject.h \
	moMotion.h \
	moNetManager.h \
	moOGLFT.h \
	moP5.h \
	moParam.h \
	moPlugin.h \
	moPostEffect.h \
	moPostPlugin.h \
	moPreConfig.h \
	moPreEffect.h \
	moPrePlugin.h \
	moPresets.h \
	moRenderManager.h \
	moResourceManager.h \
	moResourcePlugin.h \
	moSceneEffect.h \
	moScript.h \
	moScriptManager.h \
	moSequenceEffect.h \
	moShaderCG.h \
	moShaderGLSL.h \
	moShader.h \
	moShaderManager.h \
	moSoundManager.h \
	moTempo.h \
	moTextFilterParam.h \
	moText.h \
	moTextureFilter.h \
	moTextureFilterIndex.h \
	moTexture.h \
	moTextureIndex.h \
	moTextureManager.h \
	moThread.h \
	moTimeManager.h \
	moTimer.h \
	moTypes.h \
	moValue.h \
	moVdpauGraph.h \
	moVideoGraph.h \
	moVideoManager.h \
	moXmlConfig.h \
	moPort.h \
	mo3ds.h 

METASOURCES = AUTO

