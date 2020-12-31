const canvas = document.querySelector('canvas');
const gl = canvas.getContext('webgl');

// Pozycje wierzchołków piramidy 
const pozycjeWierzcholkow = [
	 0.0,  1.0,  0.0, // Przednia ściana
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
    
     0.0,  1.0,  0.0, // Prawa ściana
     1.0, -1.0,  1.0,
     1.0, -1.0, -1.0,
    
     0.0,  1.0,  0.0, // Tylna ściana
     1.0, -1.0, -1.0,
    -1.0, -1.0, -1.0,
   
     0.0,  1.0,  0.0, // Lewa ściana
    -1.0, -1.0, -1.0,
    -1.0, -1.0,  1.0
];

// Funkacja generująca losowy kolor
function randomColor() {
    return Math.random();
}

const koloryWierzcholkow = [
	
    randomColor(), randomColor(), randomColor(), 1.0,
    randomColor(), randomColor(), randomColor(), 1.0,
    randomColor(), randomColor(), randomColor(), 1.0,
    
    randomColor(), randomColor(), randomColor(), 1.0,
    randomColor(), randomColor(), randomColor(), 1.0,
    randomColor(), randomColor(), randomColor(), 1.0,
    
    randomColor(), randomColor(), randomColor(), 1.0,
    randomColor(), randomColor(), randomColor(), 1.0,
    randomColor(), randomColor(), randomColor(), 1.0,
   
    randomColor(), randomColor(), randomColor(), 1.0,
    randomColor(), randomColor(), randomColor(), 1.0,
    randomColor(), randomColor(), randomColor(), 1.0,
];



// Tworzenie bufora dla pozycji wierzchołków
const buforPozycji = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, buforPozycji);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(pozycjeWierzcholkow), gl.STATIC_DRAW);

// Tworzenie bufora dla kolorów wierzchołków
const buforKolorow = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, buforKolorow);
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(koloryWierzcholkow), gl.STATIC_DRAW);

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

// Tworzenie programu 
const program = gl.createProgram();

// Dołączanie Vertex i Fragment Shader
gl.attachShader(program, vertexShader);
gl.attachShader(program, fragmentShader);

//Linkowanie programu 
gl.linkProgram(program);

// Ustalanie miejsca składowania pozycji wektorów
const skladowaniePozycji = gl.getAttribLocation(program, `position`);
gl.enableVertexAttribArray(skladowaniePozycji);
gl.bindBuffer(gl.ARRAY_BUFFER, buforPozycji);
gl.vertexAttribPointer(skladowaniePozycji, 3, gl.FLOAT, false, 0, 0);

// Ustalanie miejsca składowania kolorów wektorów
const skladowanieKolorow = gl.getAttribLocation(program, `color`);
gl.enableVertexAttribArray(skladowanieKolorow);
gl.bindBuffer(gl.ARRAY_BUFFER, buforKolorow);
gl.vertexAttribPointer(skladowanieKolorow, 3, gl.FLOAT, false, 0, 0);

// Użycie programu 
gl.useProgram(program);
gl.enable(gl.DEPTH_TEST);

const uniformLocations = {
    modelMatrix: gl.getUniformLocation(program, `matrix`),
};

// Towrzenie macierzy obiektu i obserwatora
const modelMatrix = mat4.create();
const viewMatrix = mat4.create();

// Ustalanie perspektywy 
const projectionMatrix = mat4.create();
mat4.perspective(projectionMatrix, 120 * Math.PI/180, canvas.width/canvas.height, 1e-4, 1e4);

// Macierze pomocnicze 
const mvMatrix = mat4.create();
const mvpMatrix = mat4.create();

// Skalowanie piramidy (70% rozmiaru)
mat4.scale(modelMatrix, modelMatrix, [.7, .7, .7]);

mat4.translate(viewMatrix, viewMatrix, [0, 0.3, 1.6]);
mat4.invert(viewMatrix,viewMatrix);

// Animacja - rotacja wzg. osi Z i X
function animate() {
    requestAnimationFrame(animate);
    mat4.rotateY(modelMatrix, modelMatrix, Math.PI/2 / 90);
	mat4.multiply(mvMatrix, viewMatrix, modelMatrix);
	mat4.multiply(mvpMatrix, projectionMatrix, mvMatrix);
	gl.uniformMatrix4fv(uniformLocations.modelMatrix, false, mvpMatrix);
    gl.drawArrays(gl.TRIANGLES, 0, pozycjeWierzcholkow.length / 3);
}

//Wykonianie animacji 
animate();