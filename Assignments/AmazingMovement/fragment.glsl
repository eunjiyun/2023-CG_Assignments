#version 330

in vec3 out_Color;
out vec4 Fragcolor;


uniform vec3 lightColor; //--- ���� ���α׷����� ������ ���� ����

void main() {

	float ambientLight = 0.5; //--- �ֺ� ���� ���: 0.0 �� ambientLight �� 1.0
	vec3 ambient = ambientLight * lightColor; //--- �ֺ� ����

	vec3 result = ambient * out_Color; //--- ��ü�� ���� �ֺ������� ���Ͽ� ���� ��ü ���� ����

	//Fragcolor = vec4 (out_Color, 1.0);
	Fragcolor = vec4 (result, 1.0);
}