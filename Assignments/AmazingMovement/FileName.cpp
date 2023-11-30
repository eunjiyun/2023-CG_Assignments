#include "obj.h"



random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis2(0, 1);


GLuint shaderProg;
GLuint vao, vbo[3];
Hexahedron wall[26][26];
GLfloat cmPosY{ 0.0f };
GLfloat cmZ{ 2.0f };
GLfloat cmX{ 0.0f };
GLfloat scaleVal;
int rY{ 0 };
int perView{ 1 };
GLfloat perZ{ -5.0f };
GLchar* vertexSource, * fragmentSource;
GLuint vertexShader;
GLuint fragmentShader;
int firAni{  };
bool secAni{  };
bool thiAni{};
bool off{ true };
int lightColorLocation{};
int chooseCol{ 1 };
bool initLight{ false };
int Persona{};
int ver{}, hor{};
int upView{};
bool fir{};
bool startFall{ true };
int ani{};
int preAni{};

GLfloat vertical[1500][3] =
{
	{0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f},
	{0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f},
	{0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f},
	{0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f},
	{0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f},
	{0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f}, {0.7f, 0.7f, 0.7f},
	{0.6f, 0.6f, 0.6f}, {0.6f, 0.6f, 0.6f}, {0.6f, 0.6f, 0.6f},
	{0.6f, 0.6f, 0.6f}, {0.6f, 0.6f, 0.6f}, {0.6f, 0.6f, 0.6f},
	{0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f},
	{0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f},
	{0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f},
	{0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f}, {0.8f, 0.8f, 0.8f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f}, {0.2f, 0.2f, 0.2f},
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
	{1.0f, 0.1f, 0.1f}, {1.0f, 0.1f, 0.1f}, {1.0f, 0.1f, 0.1f},
	{1.0f, 0.3f, 0.3f}, {1.0f, 0.3f, 0.3f}, {1.0f, 0.3f, 0.3f},
	{1.0f, 0.2f, 0.2f}, {1.0f, 0.2f, 0.2f}, {1.0f, 0.2f, 0.2f},
};

float vertices[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f

};

GLfloat pos[1500][3] =
{
	{-1.0, -1.0, -1.0}, {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0},
	{1.0, -1.0, 1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0},
	{-1.0, 1.0, -1.0}, {-1.0, 1.0, 1.0}, {-1.0, -1.0, 1.0},
	{-1.0, 1.0, -1.0}, {-1.0, -1.0, 1.0}, {-1.0, -1.0, -1.0},
	{1.0, -1.0, -1.0}, {1.0, -1.0, 1.0}, {1.0, 1.0, 1.0},
	{1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, -1.0},
	{-1.0, 1.0, -1.0}, {-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0},
	{-1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}, {1.0, 1.0, -1.0},
	{-1.0, 1.0, 1.0}, {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0},
	{-1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0, 1.0, 1.0},
	{-1.0, 1.0, -1.0}, {-1.0, 1.0, 1.0}, {1.0, 1.0, 1.0},
	{1.0, 1.0, 1.0}, {1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0},
	{-1.0, -1.0, -1.0}, {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0},
	{1.0, -1.0, 1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0},
	{-1.0, 1.0, -1.0}, {-1.0, 1.0, 1.0}, {-1.0, -1.0, 1.0},
	{-1.0, 1.0, -1.0}, {-1.0, -1.0, 1.0}, {-1.0, -1.0, -1.0},
	{1.0, -1.0, -1.0}, {1.0, -1.0, 1.0}, {1.0, 1.0, 1.0},
	{1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, -1.0},
	{-1.0, 1.0, -1.0}, {-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0},
	{-1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}, {1.0, 1.0, -1.0},
	{-1.0, 1.0, 1.0}, {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0},
	{-1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0, 1.0, 1.0},
	{-1.0, 1.0, -1.0}, {-1.0, 1.0, 1.0}, {1.0, 1.0, 1.0},
	{1.0, 1.0, 1.0}, {1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0},
	{-1.0, -1.0, 1.0}, {0.0, -1.0, 0.5}, {0.0, -1.0, -1.0},
	{0.0, -1.0, 0.5}, {0.0, -1.0, -1.0}, {1.0, -1.0, 1.0},
	{-1.0, -1.0, 1.0}, {0.0, 0.0, 0.5}, {0.0, -1.0, -1.0},
	{0.0, 0.0, 0.5}, {0.0, -1.0, -1.0}, {1.0, -1.0, 1.0},
	{-1.0, -1.0, 1.0}, {0.0, -1.0, 0.5}, {0.0, 0.0, 0.5},
	{1.0, -1.0, 1.0}, {0.0, -1.0, 0.5}, {0.0, 0.0, 0.5},
};
GLvoid make_vertexShaders();
GLvoid make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid bufInit();
GLvoid shInit();
GLvoid SetCamera();
GLvoid SetProjection();
GLvoid SetTransform();
GLvoid PersonalityConversion();
GLvoid wallRender(glm::mat4, unsigned int);
GLvoid verHorFir();
GLvoid wallFir();
GLvoid firstInit();
GLvoid speedSet(int);
GLvoid drawScene(GLvoid);
GLchar* fBuffer(const char*);
GLvoid Keyboard(unsigned char, int, int);
GLvoid Timer(int);
GLvoid rectangleRender();
GLvoid help();

GLvoid main(int argc, char** argv)
{
	PlaySound(L"bgm.wav", NULL, SND_ASYNC | SND_LOOP);
	verHorFir();
	wallFir();
	help();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(800, 50);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Amazing Movement");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cerr << "Unable to initialize GLEW" << endl;
		exit(EXIT_FAILURE);
	}
	else
		cout << "GAME START!\n";

	make_vertexShaders();
	make_fragmentShaders();
	shaderProg = make_shaderProgram();
	shInit();
	bufInit();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(13, Timer, 1);
	srand(unsigned int(time(NULL)));
	rand();
	glutMainLoop();
}
GLvoid help()
{
	cout << "<Ű���� ��ɾ�>" << endl;
	cout << "1 : �ִϸ��̼� 1" << endl;
	cout << "2 : �ִϸ��̼� 2" << endl;
	cout << "3 : �ִϸ��̼� 3" << endl;
	cout << "t : ���� on/off" << endl;
	cout << "c : ���� �� ����" << endl;
	cout << "y/Y : ī�޶� �ٴ��� y���� �������� ����" << endl;
	cout << "+/- : ����ü �̵� �ӵ� ����/����" << endl;
	cout << "r : ��� �� �ʱ�ȭ" << endl;
	cout << "7 : 1��Ī ī�޶�" << endl;
	cout << "9 : 3��Ī ī�޶�" << endl;
	cout << "p : Ű���� ��ɾ �ܼ�â�� ���" << endl;
	cout << "o/O : ���� ���� / ���� ����" << endl;
	cout << "q : ���α׷� ����" << endl;
};

GLvoid Keyboard(unsigned char key, int x, int y) {

	switch (key) {
		//�ִϸ��̼� 1 2 3
	case '1':
		cout << "1 : �ִϸ��̼�1 ���" << endl;
		if (1 != preAni)
			firstInit();
		if (firAni)
			firAni = false;
		else
			firAni = true;

		preAni = 1;
		fir = true;
		break;
	case '2':
		cout << "2 : �ִϸ��̼�2 ���" << endl;
		if (2 != preAni)
			firstInit();
		if (secAni)
			secAni = false;
		else {
			secAni = true;
			float firMax = 0.5f + ((float)rand() / (RAND_MAX) / 2);
			float firMin = 0.1f + ((float)rand() / (RAND_MAX) / 4);

			for (int c{}; c < ver; ++c)
				for (int l{}; l < hor; ++l) {
					wall[c][l].where = -2;
					wall[c][l].s = 0.75f / hor * l + 0.35f;
					wall[c][l].speed = 0.000000005f / hor * (hor - l) + 0.004999995f;//�ӵ�

					wall[c][l].Smax = firMax;//�ִ� �ּ�
					wall[c][l].Smin = firMin;
				}
		}
		preAni = 2;
		fir = true;
		break;
	case '3':
		cout << "3 : �ִϸ��̼�3 ���" << endl;
		if (3 != preAni)
			firstInit();
		if (thiAni)
			thiAni = false;
		else {
			thiAni = true;

			float firVel = 0.002f + (float)rand() / (RAND_MAX) / 300;//0.002 0.005
			float secVel = 0.002f + (float)rand() / (RAND_MAX) / 300;
			float firMax = 0.5f + ((float)rand() / (RAND_MAX) / 2);
			float secMax = 0.5f + ((float)rand() / (RAND_MAX) / 2);
			float firMin = 0.1f + ((float)rand() / (RAND_MAX) / 4);
			float secMin = 0.1f + ((float)rand() / (RAND_MAX) / 4);

			for (int c{}; c < ver; ++c)
				for (int l{}; l < hor; ++l) {
					if (0 == c % 2) {//�ִϸ��̼�
						wall[c][l].where = -2;
						wall[c][l].s = 1;
						wall[c][l].speed = firVel;//�ӵ�
						wall[c][l].Smax = firMax;//�ִ� �ּ�
						wall[c][l].Smin = firMin;
					}
					else {
						wall[c][l].where = 2;
						wall[c][l].s = 0.35;
						wall[c][l].speed = secVel;
						wall[c][l].Smax = secMax;//0.5 1
						wall[c][l].Smin = secMin;//// 0.1f���� 0.35f������ ���� ����
					}
				}
		}
		preAni = 3;
		fir = true;
		break;
		//���� on off
	case 't':
		cout << "t ���� on/off" << endl;
		lightColorLocation = glGetUniformLocation(shaderProg, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���

		if (!off) {
			if (1 == chooseCol)
				glUniform3f(lightColorLocation, 0.3, 0.3, 0.3);
			else if (2 == chooseCol)
				glUniform3f(lightColorLocation, 0.3, 0.0, 0.0);
			else if (3 == chooseCol)
				glUniform3f(lightColorLocation, 0.0, 0.3, 0.0);
			else if (4 == chooseCol)
				glUniform3f(lightColorLocation, 0.0, 0.0, 0.3);
			off = true;
		}
		else {
			if (1 == chooseCol)
				glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
			else if (2 == chooseCol)
				glUniform3f(lightColorLocation, 1.0, 0.0, 0.0);
			else if (3 == chooseCol)
				glUniform3f(lightColorLocation, 0.0, 1.0, 0.0);
			else if (4 == chooseCol)
				glUniform3f(lightColorLocation, 0.0, 0.0, 1.0);
			off = false;
		}
		break;
		//���� �� ����
	case 'c':
	{
		cout << "c ���� �� ����" << endl;
		lightColorLocation = glGetUniformLocation(shaderProg, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���

		float l;
		if (off)
			l = 0.3f;
		else
			l = 1.0f;

		if (1 == chooseCol) {
			glUniform3f(lightColorLocation, l, 0.0, 0.0);
			chooseCol = 2;
		}
		else if (2 == chooseCol) {
			glUniform3f(lightColorLocation, 0.0, l, 0.0);
			chooseCol = 3;
		}
		else if (3 == chooseCol) {
			glUniform3f(lightColorLocation, 0.0, 0.0, l);
			chooseCol = 4;
		}
		else if (4 == chooseCol) {
			glUniform3f(lightColorLocation, l, l, l);
			chooseCol = 1;
		}
	}
	break;
	//ī�޶� �ٴ��� y�� ���� ��/�� ����, �ٽ� ������ ���� 
	case 'y': // �ٴ� y�� ����, �� / �� ���� ȸ��
		cout << "y ���� �������� ����/����" << endl;
		if (0 == rY)
			rY = 1;
		else
			rY = 0;
		break;

	case 'Y':
		cout << "Y ���� �������� ����/����" << endl;
		if (0 == rY)
			rY = -1;
		else
			rY = 0;
		break;
	case '+': // ����ü �̵��ϴ� �ӵ� ����
		cout << "+ ����ü �̵� �ӵ� ����" << endl;
		speedSet(1);
		break;
	case '-': // ����ü �̵��ϴ� �ӵ� ����
		cout << "- ����ü �̵� �ӵ� ����" << endl;
		speedSet(-1);
		break;
	case 'r':  // ��� �� �ʱ�ȭ
		cout << "r : ��� �� �ʱ�ȭ" << endl;
		verHorFir();
		firstInit();
		off = true;
		chooseCol = 1;
		initLight = false;
		perView = true;
		perZ = -5.0f;
		rY = 0;
		cmPosY = 0.0f;
		fir = false;
		startFall = true;
		break;
	case '7':
		cout << "7 : 1��Ī ����" << endl;
		Persona = true;
		perView = true;
		break;
	case '9':
		cout << "9 : 3��Ī ����" << endl;
		Persona = false;
		break;
	case 'p': case 'P':
		cout << "p Ű���� ��ɾ� ���" << endl;
		help();
		break;
	case 'o':
		cout << "o ��������" << endl;
		perView = false;
		break;
	case 'O':
		cout << "O ��������" << endl;
		perView = true;
		break;
	case 'Q': case 'q':
		cout << "q ���α׷� ����" << endl;
		exit(0);
		break;
		glutPostRedisplay();
	}
}

GLvoid verHorFir() {
	cout << "�� �Է� : ";
	cin >> hor;

	cout << "�� �Է� : ";
	cin >> ver;

	if (!(5 <= hor and 25 >= hor)) {
		cout << "�߸��� ���Դϴ�!" << endl;
		verHorFir();
	}

	scaleVal = 2.0f / hor;
	if (0 >= scaleVal)
		cout << "���� �߻�!" << endl;
}

GLvoid wallFir() {
	for (int c{}; c < ver; ++c) {
		for (int l{}; l < hor; ++l) {

			wall[c][l].ver = c;
			wall[c][l].hor = l;
			wall[c][l].width = scaleVal, wall[c][l].height = scaleVal;
			wall[c][l].canSee = true;
			wall[c][l].is = true;
			wall[c][l].aniSt = 10.f;

			switch (dis2(gen)) {
			case 0:
				wall[c][l].where = -2;
				break;
			case 1:
				wall[c][l].where = 2;
				break;
			}

			wall[c][l].Smax = 0.5f + ((float)rand() / (RAND_MAX) / 2);//0.5 1
			wall[c][l].Smin = 0.1f + ((float)rand() / (RAND_MAX) / 4);//// 0.1f���� 0.35f������ ���� ����

			if (wall[c][l].Smax < wall[c][l].Smin)
				cout << "���� �߻�!" << endl;

			wall[c][l].s = wall[c][l].Smax - 0.2f;
			wall[c][l].speed = 0.002f + (float)rand() / (RAND_MAX) / 300;
		}
	}
}


GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProg);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 1000, 1000);
	upView = false;
	SetCamera();
	SetProjection();
	if (Persona)
		PersonalityConversion();
	else SetTransform();
	if (Persona)
		glDisable(GL_DEPTH_TEST);
	glViewport(700, 700, 300, 300);
	upView = true;
	SetCamera();
	SetProjection();
	SetTransform();
	glBindVertexArray(vao);
	glutSwapBuffers();
}


GLvoid PersonalityConversion()
{
	glm::mat4 TR = glm::mat4(1.0f);
	unsigned int modelLocation = glGetUniformLocation(shaderProg, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	TR = glm::rotate(TR, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	TR = glm::translate(TR, glm::vec3(0.0f, -1.0f, 0.0f));
	TR = glm::scale(TR, glm::vec3(1.0f, 0.02f, 1.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	for (int i{ 36 }; i < 70; i += 3)
		glDrawArrays(GL_TRIANGLES, i, 3);


	TR = glm::scale(TR, glm::vec3(1.0f, 1.0f / 0.02f, 1.0f));
	TR = glm::translate(TR, glm::vec3(0.0f, 1.0f, 0.0f));

	wallRender(TR, modelLocation);
}



GLvoid wallRender(glm::mat4 tran, unsigned int modelLocation)
{
	GLfloat ScaleValue{ scaleVal / 2 };

	for (int c{}; c < ver; ++c) {
		for (int l{}; l < hor; ++l) {
			if (wall[c][l].canSee and wall[c][l].is) {
				GLfloat Xvalue = -1.0f + (scaleVal / 2) + (scaleVal * wall[c][l].hor);
				GLfloat Zvalue = 1.0f - (scaleVal / 2) - (scaleVal * wall[c][l].ver);

				tran = glm::translate(tran, glm::vec3(Xvalue, 0, Zvalue));
				tran = glm::scale(tran, glm::vec3(ScaleValue, 1.0f, ScaleValue));
				tran = glm::translate(tran, glm::vec3(0.0f, -1.0f, 0.0f));
				tran = glm::scale(tran, glm::vec3(1.0f, wall[c][l].s, 1.0f));
				tran = glm::translate(tran, glm::vec3(0.0f, 1.0f, 0.0f));

				if (!fir)
					tran = glm::translate(tran, glm::vec3(0, 0.05f * wall[c][l].aniSt, 0));

				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(tran));

				rectangleRender();

				tran = glm::translate(tran, glm::vec3(0.0f, -1.0f, 0.0f));
				tran = glm::scale(tran, glm::vec3(1.0f, 1.0f / wall[c][l].s, 1.0f));
				tran = glm::translate(tran, glm::vec3(0.0f, 1.0f, 0.0f));

				tran = glm::scale(tran, glm::vec3(1.0f / ScaleValue, 1.0f, 1.0f / ScaleValue));
				tran = glm::translate(tran, glm::vec3(-Xvalue, 0.0f, -Zvalue));
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(tran));
			}
		}
	}

}

GLvoid rectangleRender()
{
	for (int i{}; i < 34; i += 3)
		glDrawArrays(GL_TRIANGLES, i, 3);
}

GLvoid SetCamera()
{
	glm::mat4 VR = glm::mat4(1.0f);
	glm::vec3 cameraPos = glm::vec3(cmX, 0.0f, cmZ);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	VR = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	unsigned int viewLocation = glGetUniformLocation(shaderProg, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(VR));
}

GLvoid SetProjection() {
	glm::mat4 PR = glm::mat4(1.0f);
	unsigned int projLocation = glGetUniformLocation(shaderProg, "projectionTransform");

	if (perView)
		PR = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	else
		PR = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, -2.0f, 100.0f);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(PR));
}

GLvoid speedSet(int d)
{
	for (int c{}; c < ver; ++c) {
		for (int l{}; l < hor; ++l) {
			if (wall[c][l].is) {
				if (-1 == d) {
					if (0.0f < wall[c][l].speed - 0.0005f)
						wall[c][l].speed -= 0.0005f;
				}
				else if (1 == d) {
					if (0.05f > wall[c][l].speed + 0.001f)
						wall[c][l].speed += 0.001f;
				}
			}
		}
	}
}

GLvoid firstInit()
{
	wallFir();

	for (int i{}; i < ver; ++i)
		for (int j{}; j < hor; ++j)
			wall[i][j].aniSt = 10.f;

	perView = 1;
	perZ = -5.0f;
	firAni = false;
	secAni = false;
	thiAni = false;
	Persona = false;
	cmZ = 2.0f;
	cmX = 0.0f;
}

GLvoid Timer(int value)
{
	if (startFall) {
		ani += 3;
		for (int i{}; i < ver; ++i)
			for (int j{}; j < hor; ++j) {
				if (ani >= i * j + j && wall[i][j].aniSt > 0.f)
					wall[i][j].aniSt -= 0.3f;
				if (wall[i][j].aniSt <= 0.f)
					wall[i][j].aniSt = 0.f;
			}
		if (wall[ver - 1][hor - 1].aniSt == 0.f)
			startFall = false;
	}

	for (int c{}; c < ver; ++c) {
		for (int l{}; l < hor; ++l) {
			if (wall[c][l].canSee and wall[c][l].is) {
				if (firAni) {
					if (-2 == wall[c][l].where) {
						if (wall[c][l].s <= wall[c][l].Smin)
							wall[c][l].where *= -1;
						else
							wall[c][l].s -= wall[c][l].speed;
					}
					else if (2 == wall[c][l].where) {
						if (wall[c][l].s >= wall[c][l].Smax)
							wall[c][l].where *= -1;
						else
							wall[c][l].s += wall[c][l].speed;
					}
				}
				else if (secAni) {
					if (-2 == wall[c][l].where) {
						if (wall[c][l].s <= wall[c][l].Smin)
							wall[c][l].where *= -1;
						else
							wall[c][l].s -= wall[c][l].speed;
					}
					else if (2 == wall[c][l].where) {
						if (wall[c][l].s >= wall[c][l].Smax)
							wall[c][l].where *= -1;
						else
							wall[c][l].s += wall[c][l].speed;
					}
				}
				else if (thiAni) {
					if (-2 == wall[c][l].where) {
						if (wall[c][l].s <= wall[c][l].Smin)
							wall[c][l].where *= -1;
						else
							wall[c][l].s -= wall[c][l].speed;
					}
					else if (2 == wall[c][l].where) {
						if (wall[c][l].s >= wall[c][l].Smax)
							wall[c][l].where *= -1;
						else
							wall[c][l].s += wall[c][l].speed;
					}
				}
			}
		}
	}

	if (1 == rY)
		cmPosY += 0.3f;
	else if (-1 == rY)
		cmPosY -= 0.3f;

	glutPostRedisplay();
	bufInit();
	glutTimerFunc(13, Timer, 1);
}




GLuint make_shaderProgram()
{
	GLuint ShaderProgramID;
	ShaderProgramID = glCreateProgram();
	glAttachShader(ShaderProgramID, vertexShader);
	glAttachShader(ShaderProgramID, fragmentShader);
	glLinkProgram(ShaderProgramID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
	if (not result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program ���� ����\n" << errorLog << endl;
	}
	glUseProgram(ShaderProgramID);
	return ShaderProgramID;
}
GLvoid bufInit()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(3, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 500 * 9 * sizeof(GLfloat), pos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 500 * 9 * sizeof(GLfloat), vertical, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //--- ��ġ �Ӽ�
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //--- �븻 �Ӽ�
	glEnableVertexAttribArray(3);
	glUseProgram(shaderProg);

	unsigned int lightPosLocation = glGetUniformLocation(shaderProg, "lightPos"); //--- lightPos �� ����: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 0.0, 5.0, 1.0);

	if (not initLight) {
		lightColorLocation = glGetUniformLocation(shaderProg, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���
		glUniform3f(lightColorLocation, 0.3, 0.3, 0.3);
		initLight = true;
	}
	unsigned int viewPosLocation = glGetUniformLocation(shaderProg, "viewPos"); //--- viewPos �� ����: ī�޶� ��ġ
	glUniform3f(viewPosLocation, cmX, cmPosY, cmZ);
}

GLvoid shInit()
{
	make_vertexShaders();
	make_fragmentShaders();
	shaderProg = glCreateProgram();
	glAttachShader(shaderProg, vertexShader);
	glAttachShader(shaderProg, fragmentShader);
	glLinkProgram(shaderProg);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glUseProgram(shaderProg);
}

GLchar* fBuffer(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;

	fptr = fopen(file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;

	return buf;
}

GLvoid make_vertexShaders()
{
	vertexSource = fBuffer("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (not result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
		return;
	}
}

GLvoid make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = fBuffer("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (not result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
		return;
	}
}

GLvoid SetTransform() {
	glm::mat4 tran = glm::mat4(1.0f);

	unsigned int modelLocation = glGetUniformLocation(shaderProg, "modelTransform");

	if (perView) {
		if (upView)
			tran = glm::translate(tran, glm::vec3(0.0, 0.0, -2.0));
		else
			tran = glm::translate(tran, glm::vec3(0.0, 0.0, perZ));
	}

	tran = glm::translate(tran, glm::vec3(-cmX * 2, 0.0f, 0.0));
	tran = glm::translate(tran, glm::vec3(cmX * 2, 0.0f, 0.0));


	if (upView) {
		tran = glm::rotate(tran, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else tran = glm::rotate(tran, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	tran = glm::rotate(tran, glm::radians(cmPosY), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(tran));


	tran = glm::translate(tran, glm::vec3(0.0f, -1.0f, 0.0f));
	tran = glm::scale(tran, glm::vec3(1.0f, 0.02f, 1.0f));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(tran));
	for (int i{ 36 }; i < 70; i += 3)
		glDrawArrays(GL_TRIANGLES, i, 3);
	tran = glm::scale(tran, glm::vec3(1.0f, 1.0f / 0.02f, 1.0f));
	tran = glm::translate(tran, glm::vec3(0.0f, 1.0f, 0.0f));
	wallRender(tran, modelLocation);
}

