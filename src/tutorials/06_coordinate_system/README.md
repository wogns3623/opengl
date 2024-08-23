# Coordinate system

## NDC(Normalized Device Coordinates)

- 최종적으로 rasterize되어 화면을 그릴 때 사용될 좌표 시스템

## Transformation steps

### Local space(Object space)

- 각각의 오브젝트의 좌표

### World space

- 오브젝트들이 배치된 공간 좌표
- 각 오브젝트별로 위치, 크기, 회전을 위한 변환 행렬인 model matrix를 적용해 world space로 변환함

### View space(Eye space)

- 공간을 카메라 시점에서 본 좌표
- world space에 view matrix라고 불리는 행렬을 적용해 변환함

### Clip space

- vertex shader가 사용할 수 있도록 -1 ~ 1 사이의 값으로 변환된 좌표

### Screen space

- 화면을 그릴 때 사용할 좌표
- -1 ~ 1 사이의 좌표를 창의 좌표범위로 변환하는 viewport 변환 작업을 통해 생성됨
