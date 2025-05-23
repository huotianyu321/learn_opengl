#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// 纹理采样器
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	//在两个纹理像素(Texture pixel, Texel)的颜色间线性插值, 第三个参数是第二个纹理的像素颜色百分比
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}