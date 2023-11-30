#version 330

in vec3 out_Color;
out vec4 Fragcolor;

in vec3 FragPos; //--- ��ġ��
in vec3 Normal; //--- ���ؽ� ���̴����� ���� ��ְ�


uniform vec3 lightPos; //--- ������ ��ġ
uniform vec3 lightColor; //--- ���� ���α׷����� ������ ���� ����

void main() {

	float ambientLight = 0.3; //--- �ֺ� ���� ���: 0.0 �� ambientLight �� 1.0
	vec3 ambient = ambientLight * lightColor; //--- �ֺ� ����

	vec3 normalVector = normalize(Normal); //--- �븻���� ����ȭ�Ѵ�.
	vec3 lightDir = normalize(lightPos-vec3(FragPos.x,FragPos.y,FragPos.z)); //--- ǥ��� ������ ��ġ�� ������ ������ �����Ѵ�.
	float diffuseLight = max(dot (Normal, lightDir), 0.0); //--- N�� L�� ���� ������ ���� ���� (���� ���� ���� �� ���� �Ѵ�.)
	float diffuse = diffuseLight * lightColor.x; //--- ����ݻ�����=����ݻ簪*�������

	vec3 result = (ambient + diffuse) * out_Color; //--- ���� ���� ������ �ȼ� ����=(�ֺ�����+����ݻ�����)*��ü ����
	//vec3 result = ambient * out_Color; //--- ��ü�� ���� �ֺ������� ���Ͽ� ���� ��ü ���� ����
	//Fragcolor = vec4 (out_Color, 1.0);

	Fragcolor = vec4 (result, 1.0);
}