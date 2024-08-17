# GLSL language

started with version
`#version 330 core`

## Shader input

`in type input_variable_name;`

In vertex shader, input is vertex attribute

There is maximum limit of vertex attributes we're allowed to declare

OpenGL guarantees least 16 4-component

Can get maximum number by using `glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);`

## Shader output

`out type output_variable_name;`

Fragment shader must define output variable with vec4 cause it need to generate final output color rgba

## Uniforms

Another way to pass data from application(on cpu) to shader(on gpu)

Unlike input variables, uniform variables are unique per shader program object

- Thus, uniform variables can be accessed any shaders at any stages in shader program

And keep their value until they are reset or update

You only need to define a uniform variable in the shader that will use it

- If you declare a uniform that isn't used anywhere in shader, GLSL compiler will silently remove that variable from output witch may cause various issues

## Variable type

Contain basic C types(int, float, double, uint...)

Also it has widely used container type: vectors, matrices

### Vector

N is number of vector components (2~4)

- vecN: float
- bvecN: boolean
- ivecN: int
- uvecN: unsigned int
- dvecN: double

`new_vec = vec4(0.1, 0.1, 1.0, 1.0);`

Vector component can be accessed via xyzw(coordinate) / rgba(color) / stpq(texture)

#### Swizzling

GLSL can make new vector using existing's components

For example, `some_vec.xxyx` create new vector {x, x, y, x}

- Can create different size vector using this `new_vec2 = some_vec4.xz`
- `new_vec = some_vec.xyz + some_other_vec.xxx;` is also possible
- Created vector has same type with original
- Can use swizzling as arguments `new_vec = vec4(some_vec.rgb, 1.0);`
- `new_vec = vec4(some_vec, 1.0);` is also possible
