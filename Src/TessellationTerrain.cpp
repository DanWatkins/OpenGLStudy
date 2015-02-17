#include <QtGui/QGuiApplication>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLTexture>
#include <QKeyEvent>

#include <Common/OpenGLWindow.h>


int randInt()
{
	return rand();
}


int randInt(int a, int b)
{
	return rand() % (b-a+1) + a;
}


class TessellationTerrain : public OpenGLWindow
{
public:
    void initialize() override
	{
		OpenGLWindow::initialize();

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

		//grab uniform locations
		{
			mUniforms.mvMatrix = mProgram.uniformLocation("mvMatrix");
			mUniforms.mvpMatrix = mProgram.uniformLocation("mvpMatrix");
			mUniforms.projMatrix = mProgram.uniformLocation("projMatrix");
			mUniforms.dmapDepth = mProgram.uniformLocation("dmapDepth");
		}

		mVao.bind();
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//build displacment map
		{
			QVector<GLubyte> displacmentMap;
			displacmentMap.reserve(mSize*mSize);

			std::srand(35456);
		
			for (int i=0; i<mSize*mSize; i++)
				displacmentMap.append(randInt(0, 255));

			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, mSize, mSize);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mSize, mSize, GL_R, GL_UNSIGNED_BYTE, &displacmentMap[0]);
		}

		//load terrain texture
		{
			glActiveTexture(GL_TEXTURE1);
			mTerrainTexture = QSharedPointer<QOpenGLTexture>(new QOpenGLTexture(QImage("./Common/dirt.png").mirrored()));
			mTerrainTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
			mTerrainTexture->setMagnificationFilter(QOpenGLTexture::Linear);
		}
	}


	void render() override
	{
		//general
		{
			OpenGLWindow::render();
			const qreal retinaScale = devicePixelRatio();
			glViewport(0, 0, width() * retinaScale, height() * retinaScale);
			

			static const GLfloat black[] = { 0.85f, 0.95f, 1.0f, 1.0f };
			static const GLfloat one = 1.0f;

			glClearBufferfv(GL_COLOR, 0, black);
			glClearBufferfv(GL_DEPTH, 0, &one);
		}

		mProgram.bind();
		mVao.bind();
		{
			static double totalTime = 0.0;
			totalTime += 0.01;

			float t = (float)totalTime * 0.03f;
			float r = sinf(t * 5.37f) * 15.0f + 16.0f;
			float h = cosf(t * 4.79f) * 2.0f + 3.2f;

			Mat4 mv_matrix = glm::lookAt(Vec3(30, 10, 30), Vec3(0.0f), Vec3(0.0f, 1.0f, 0.0f));
			Mat4 proj_matrix = glm::perspective(60.0f, (float)width() / (float)height(), 0.1f, 1000.0f);

			glUniformMatrix4fv(mUniforms.mvMatrix, 1, GL_FALSE, glm::value_ptr(mv_matrix));
			glUniformMatrix4fv(mUniforms.projMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix));
			glUniformMatrix4fv(mUniforms.mvpMatrix, 1, GL_FALSE, glm::value_ptr(proj_matrix * mv_matrix));
			glUniform1f(mUniforms.dmapDepth, 6.0f);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);

			glPolygonMode(GL_FRONT_AND_BACK, mWireframe ? GL_LINE : GL_FILL);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);
			glActiveTexture(GL_TEXTURE1);
			mTerrainTexture->bind();
			glDrawArraysInstanced(GL_PATCHES, 0, 4, mSize*mSize);
		}
		mVao.release();
		mProgram.release();
	}


	void keyPressEvent(QKeyEvent *ev) override
	{
		OpenGLWindow::keyPressEvent(ev);

		switch (ev->key())
		{
		case Qt::Key_W:
			mWireframe = !mWireframe; break;
		case Qt::Key_Escape:
			OpenGLWindow::close(); break;
		}
	}


private:
    QOpenGLShaderProgram mProgram;
	QOpenGLVertexArrayObject mVao;
	const int mSize = 64;
	QSharedPointer<QOpenGLTexture> mTerrainTexture;
	GLuint tex;
	bool mWireframe = false;

	struct
	{
		GLint mvpMatrix;
		GLint mvMatrix;
		GLint projMatrix;
		GLint dmapDepth;
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

    TessellationTerrain window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app.exec();
}