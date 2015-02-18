#include <QtGui/QGuiApplication>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLTexture>
#include <QKeyEvent>

#include <Common/OpenGLWindow.h>
#include <Common/Camera.h>

class App : public OpenGLWindow
{
public:
    void initialize() override
	{
		OpenGLWindow::initialize();

		//load shaders
		{
			QOpenGLShader vs(QOpenGLShader::Vertex);
				vs.compileSourceFile("BasicTessellationTerrain/main.vs.glsl");
				mProgram.addShader(&vs);

			QOpenGLShader tcs(QOpenGLShader::TessellationControl);
				tcs.compileSourceFile("BasicTessellationTerrain/main.tcs.glsl");
				mProgram.addShader(&tcs);

			QOpenGLShader tes(QOpenGLShader::TessellationEvaluation);
				tes.compileSourceFile("BasicTessellationTerrain/main.tes.glsl");
				mProgram.addShader(&tes);

			QOpenGLShader fs(QOpenGLShader::Fragment);
				fs.compileSourceFile("BasicTessellationTerrain/main.fs.glsl");
				mProgram.addShader(&fs);

			if (!mProgram.link())
				qFatal("Error linking shaders");

			mProgram.bind();
		}

		//grab uniform locations
		{
			GLint loc = mProgram.uniformLocation("terrainSize");
			glUniform1i(loc, mSize);
			glUniform1f(mProgram.uniformLocation("height"), mHeight);
			mUniforms.mvMatrix = mProgram.uniformLocation("mvMatrix");
			mUniforms.projMatrix = mProgram.uniformLocation("projMatrix");
		}

		//build displacment map
		{
			QVector<float> displacmentMap;
			displacmentMap.reserve(mSize*mSize);

			std::srand(35456);
			float base = (mSize-1)*2;

			for (int y=0; y<mSize; y++)
			{
				for (int x=0; x<mSize; x++)
					displacmentMap.append(randFloat(0.0, 1.0));
			}

			//smooth
			for (int i=0; i<displacmentMap.count()-1; i++)
			{
				float next = displacmentMap[i+1];
				displacmentMap[i] = (displacmentMap[i]+next)/2.0f;
			}

			glGenTextures(1, &tex);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, mSize, mSize);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mSize, mSize, GL_RED, GL_FLOAT, &displacmentMap[0]);
		}

		mVao.create();
		mVao.bind();
		{
			glPatchParameteri(GL_PATCH_VERTICES, 4);
		}
	}


	void render() override
	{
		//general
		{
			OpenGLWindow::render();
			const qreal retinaScale = devicePixelRatio();
			glViewport(0, 0, width() * retinaScale, height() * retinaScale);
			

			static const GLfloat black[] = { 0.0f, 0.4f, 0.2f, 1.0f };
			static const GLfloat one = 1.0f;

			glClearBufferfv(GL_COLOR, 0, black);
			glClearBufferfv(GL_DEPTH, 0, &one);
		}

		mProgram.bind();
		mVao.bind();
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);

			glUniformMatrix4fv(mUniforms.mvMatrix, 1, GL_FALSE, glm::value_ptr(mCamera.viewMatrix()));
			glUniformMatrix4fv(mUniforms.projMatrix, 1, GL_FALSE, glm::value_ptr(mCamera.perspectiveMatrix()));

			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LEQUAL);

			glPolygonMode(GL_FRONT_AND_BACK, mWireframe ? GL_LINE : GL_FILL);
			glDrawArraysInstanced(GL_PATCHES, 0, 4, mSize*mSize);
		}
		mVao.release();
		mProgram.release();
	}


	void keyPressEvent(QKeyEvent *ev) override
	{
		OpenGLWindow::keyPressEvent(ev);
		const float speed = 0.1f;
		const float rotSpeed = 0.3f;

		switch (ev->key())
		{
		case Qt::Key_F1:
			mWireframe = !mWireframe; break;
		case Qt::Key_Escape:
			OpenGLWindow::close(); break;
			
		case Qt::Key_W:
			mCamera.incPosition(speed * mCamera.forward()); break;
		case Qt::Key_S:
			mCamera.incPosition(speed * -mCamera.forward()); break;
		case Qt::Key_A:
			mCamera.incPosition(speed * -mCamera.right()); break;
		case Qt::Key_D:
			mCamera.incPosition(speed * mCamera.right()); break;

		case Qt::Key_Q:
			mCamera.incOrientation(-rotSpeed, 0.0f); break;
		case Qt::Key_E:
			mCamera.incOrientation(rotSpeed, 0.0f); break;
		case Qt::Key_R:
			mCamera.incOrientation(0.0f, rotSpeed); break;
		case Qt::Key_F:
			mCamera.incOrientation(0.0f, -rotSpeed); break;
		}
	}


private:
    QOpenGLShaderProgram mProgram;
	QOpenGLVertexArrayObject mVao;
	const int mSize = 16;
	const float mHeight = 10.0f;
	GLuint tex;
	bool mWireframe = false;

	Camera mCamera;

	struct
	{
		GLint terrainSize;
		GLint mvMatrix;
		GLint projMatrix;
	} mUniforms;
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

    App window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app.exec();
}