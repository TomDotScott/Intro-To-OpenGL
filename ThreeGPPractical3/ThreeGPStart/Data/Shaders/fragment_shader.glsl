#version 330 core

out vec3 color;

in vec3 varying_colour;

void main()
{
  color = varying_colour;
}