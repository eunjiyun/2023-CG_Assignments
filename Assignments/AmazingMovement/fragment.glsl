#version 330

in vec3 out_Color;
out vec4 Fragcolor;

in vec3 FragPos; //--- ��ġ��
in vec3 Normal; //--- ���ؽ� ���̴����� ���� ��ְ�


uniform vec3 lightPos; //--- ������ ��ġ
uniform vec3 lightColor; //--- ���� ���α׷����� ������ ���� ����

uniform vec3 viewPos; //--- viewPos �� ����: ī�޶� ��ġ


void main() {

	float ambientLight = 1.0; //--- �ֺ� ���� ���: 0.0 �� ambientLight �� 1.0
	vec3 ambient = ambientLight * lightColor; //--- �ֺ� ����

	vec3 normalVector = normalize(Normal); //--- �븻���� ����ȭ�Ѵ�.
	vec3 lightDir = normalize(lightPos-vec3(FragPos.x,FragPos.y,FragPos.z)); //--- ǥ��� ������ ��ġ�� ������ ������ �����Ѵ�.
	float diffuseLight = max(dot (Normal, lightDir), 0.0); //--- N�� L�� ���� ������ ���� ���� (���� ���� ���� �� ���� �Ѵ�.)

	float lc;
	if(lightColor.x>0)
		lc=lightColor.x;
	else if(lightColor.y>0)
		lc=lightColor.y;
	else if(lightColor.z>0)
		lc=lightColor.z;
	
	float diffuse = diffuseLight * lc; //--- ����ݻ�����=����ݻ簪*�������

	int shininess = 128; //--- ���� ���
	vec3 viewDir = normalize(viewPos-vec3(FragPos.x,FragPos.y,FragPos.z)); //--- �������� ����
	vec3 reflectDir = reflect(-lightDir, normalVector); //--- �ݻ� ����: reflect �Լ� - �Ի� ������ �ݻ� ���� ���
	float specularLight = max (dot (viewDir, reflectDir), 0.0); //--- V�� R�� ���������� ���� ����: ���� ����
	specularLight = pow(specularLight, shininess); //--- shininess ���� ���־� ���̶���Ʈ�� ������ش�.
	vec3 specular = vec3(specularLight * lightColor.x,specularLight * lightColor.y,specularLight * lightColor.z); //--- �ſ� �ݻ� ����: �ſ�ݻ簪 * �������


	vec3 result = (ambient + diffuse + specular) * out_Color; //--- ���� ���� ������ �ȼ� ����: (�ֺ�+����ݻ�+�ſ�ݻ�����)*��ü ����
	//vec3 result = (ambient + diffuse) * out_Color; //--- ���� ���� ������ �ȼ� ����=(�ֺ�����+����ݻ�����)*��ü ����
	//vec3 result = ambient * out_Color; //--- ��ü�� ���� �ֺ������� ���Ͽ� ���� ��ü ���� ����
	//Fragcolor = vec4 (out_Color, 1.0);

	Fragcolor = vec4 (result, 1.0);
}