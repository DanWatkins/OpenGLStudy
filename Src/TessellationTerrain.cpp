#include <QtGui/QGuiApplication>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLVertexArrayObject>
#include <Common/OpenGLWindow.h>

class TessellationTerrain : public OpenGLWindow
{
public:
    void initialize() override
	{
		//load shaders
		{
			QOpenGLShader vs(QOpenGLShader::Vertex);
				vs.compileSourceFile("TessellationTerrain/main.vs.glsl");
				mProgram.addShader(&vs);

			QOpenGLShader tcs(QOpenGLShader::TessellationControl);
				tcs.compileSourceFile("TessellationTerrain/main.tcs.glsl");
				mProgram.addShader(&tcs);

			QOpenGLShader tes(QOpenGLShader::TessellationEvaluation);
				tes.compileSourceFile("TessellationTerrain/main.tes.glsl");
				mProgram.addShader(&tes);

			QOpenGLShader fs(QOpenGLShader::Fragment);
				fs.compileSourceFile("TessellationTerrain/main.fs.glsl");
				mProgram.addShader(&fs);

			if (!mProgram.link())
				qFatal("Error linking shaders");
		}

		mVao.bind();
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//build displacment map
		{
			const int size = 64;
			mDisplacmentMap.reserve(size*size);
		
			for (int i=0; i<size*size; i++)
				mDisplacmentMap[i] = rand() / (RAND_MAX + 1.0);
		}
	}


	void render() override
	{
		const qreal retinaScale = devicePixelRatio();
		glViewport(0, 0, width() * retinaScale, height() * retinaScale);

		glClear(GL_COLOR_BUFFER_BIT);
    
		mProgram.bind();
		{
			mVao.bind();
			glDrawArrays(GL_PATCHES, 0, 4);
		}
		mProgram.release();
	}

private:
    QOpenGLShaderProgram mProgram;
	QOpenGLVertexArrayObject mVao;

	QVector<float> mDisplacmentMap;
};

int main(int argc, char **argv)
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    TessellationTerrain window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app.exec();
}