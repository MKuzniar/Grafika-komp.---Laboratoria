const canvas = document.querySelector('canvas');
const gl = canvas.getContext('webgl');

// Pozycje wierzchołków trójkąta (równobocznego)
const pozycjaWierzcholkow = [
    -1, -.866, 0,    // wierzchołek 1 (x, y, z)
     1, -.866, 0,    // wierzchołek 2 (x, y, z)
     0,     1, 0,    // wierzchołek 3 (x, y, z)
];

// Kolory wierzchołków trójkąta
const kolorWierzcholkow = [
    0.196078,     0.8, 0.196078, // wierzchołek 1 (R, G, B)
	0.576471,0.858824, 0.439216, // wierzchołek 2 (R, G, B)
	    0.90,    0.91,     0.98  // wierzchołek 3 (R, G, B)
];

// Tworzenie bufora dla pozycji wierzchołków 
const buforPozycji = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, buforPozycji);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(pozycjaWierzcholkow), gl.STATIC_DRAW);

// Tworzenie bufora dla koloru wierzchołków 
const buforKoloru = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, buforKoloru);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(kolorWierzcholkow), gl.STATIC_DRAW);

// Towrzenie Vertex Shader
const vertexShader = gl.createShader(gl.VERTEX_SHADER);
gl.shaderSource(vertexShader, `
precision mediump float;

attribute vec3 position;
attribute vec3 color;
varying vec3 vColor;

uniform mat4 matrix;

void main() {
    vColor = color;
    gl_Position = matrix * vec4(position, 1);
}
`);
gl.compileShader(vertexShader);

// Tworzenie Fragment Shader
const fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
gl.shaderSource(fragmentShader, `
precision mediump float;

varying vec3 vColor;

void main() {
    gl_FragColor = vec4(vColor, 1);
}
`);
gl.compileShader(fragmentShader);

// Towrzenie programu 
const program = gl.createProgram();

// Dołączanie Vertex i Fragment Shader
gl.attachShader(program, vertexShader);
gl.attachShader(program, fragmentShader);

// Linkowanie programu 
gl.linkProgram(program);

// Ustalanie miejsca składowania pozycji wektorów
const skladowaniePozycji = gl.getAttribLocation(program, `position`);
gl.enableVertexAttribArray(skladowaniePozycji);
gl.bindBuffer(gl.ARRAY_BUFFER, buforPozycji);
gl.vertexAttribPointer(skladowaniePozycji, 3, gl.FLOAT, false, 0, 0);

//Ustalenie miejsca składowania kolorów wektorów 
const skladowanieKoloru = gl.getAttribLocation(program, `color`);
gl.enableVertexAttribArray(skladowanieKoloru);
gl.bindBuffer(gl.ARRAY_BUFFER, buforKoloru);
gl.vertexAttribPointer(skladowanieKoloru, 3, gl.FLOAT, false, 0, 0);

// Użycie programu 
gl.useProgram(program);

const uniformLocations = {
    matrix: gl.getUniformLocation(program, `matrix`),
};

const matrix = mat4.create();

// Skalowanie trójkąta (70% rozmiaru)
mat4.scale(matrix, matrix, [.7, .7, .7]);

// Animacja - rotacja wzg. osi Z trójkąta
function animate() {
    requestAnimationFrame(animate);
    mat4.rotateZ(matrix, matrix, Math.PI/2 / 80);
    gl.uniformMatrix4fv(uniformLocations.matrix, false, matrix);
    gl.drawArrays(gl.TRIANGLES, 0, 3);
}

//Wykonianie animacji 
animate();
