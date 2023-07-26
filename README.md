# Informatica_grafica_visual-OpenGL-Juego_tower_defense
## Instrucciones para la realización del proyecto
### Objetivo
Desarrollar una aplicación gráfica interactiva que abarque los distintos conceptos vistos en los guiones de prácticas.  Se  realizará  el  diseño  e  implementación  de  un  proyecto  en  el  que  se  incluirán  los  conceptos explicados a lo largo del curso, utilizando las clases proporcionadas en las plantillas en C++ con las librerías OpenGL y glut. 

### Entorno de desarrollo 
El entorno de trabajo en el que se desarrollarán las prácticas será el de *Microsoft Visual Studio Community*, que es el entorno utilizado en los laboratorios de prácticas, siendo recomendable utilizar la misma versión que  esté  instalada  en  el  aula  para  que  no  haya  problemas  de  compatibilidad.  Esto  quiere  decir  que  las prácticas que se entreguen deben poder compilarse y utilizarse en un proyecto de este entorno de desarrollo *sin necesidad de cambios*. Aquellas prácticas que no se entreguen de este modo no serán evaluadas. Se propone como alternativa la utilización de *Xcode* como entorno de desarrollo para *Mac*. Las plantillas están preparadas para su funcionamiento en este entorno. 

Existe otra alternativa que consiste en utilizar *Netbeans* para  el  desarrollo,  pero  el proyecto  debe  migrarse  previamente  a  Microsoft  Visual  Studio Community (versión del aula) o Xcode para su entrega, comprobando el alumno que funciona correctamente en el aula de prácticas  antes  de  su  entrega.  No  se  corregirán  proyectos  en  Netbeans  u  otros  entornos  de desarrollo o plataformas diferentes. 

### Requisitos que se deben cumplir
- Se deben incluir distintas vistas de la escena generada, permitiendo cambiar entre vistas, así como cambiar la posición de la cámara de manera interactiva. Es común que haya vistas de planta, alzado y perfil, así como una vista que permita moverse por la escena de manera interactiva. A veces, una vista en primera persona puede ser adecuada. Se debe incluir la posibilidad de cambio del tipo de proyección, paralela o perspectiva, así como dividir la ventana en varias vistas.
  
- Entre los movimientos de la cámara deben implementarse los de zoom, órbita, cabeceo, panorámica y desplazamiento.
  
- Se deben modelar objetos complejos en la escena, mediante primitivas de OpenGL o mallas de triángulos. Las mallas de triángulos deben usar arrays de vértices y de normales de OpenGL.
  
- Deben poder realizarse transformaciones de los objetos de la escena de manera interactiva mediante el uso de la selección del objeto y movimiento del ratón. Estas transformaciones deben realizarse utilizando la pila de matrices de OpenGL de manera adecuada, siendo necesaria la combinación de transformaciones de traslación, rotación y escalado
  
- Debe usarse un grafo de escena para una de las figuras como mínimo, que permita su interacción con las partes móviles mediante la selección de la parte y movimientos mediante el ratón. La figura del grafo de escena no puede ser la misma que se haya desarrollado en las prácticas guiadas y debe contar con un mínimo  de  3  partes  móviles  dependientes  que  combinen  traslaciones,  rotaciones  y  escalados.  Se valorará el realismo del movimiento, impidiéndose posiciones que no sean realistas. Se debe incluir la representación del grafo de escena en un documento pdf que incluya las transformaciones locales y de instancia de cada nodo.

- Puede utilizarse menús para la selección de modos de funcionamiento. La interacción debe enfocarse en su mayor parte a la selección de objetos mediante ratón y transformaciones o movimientos mediante el desplazamiento del ratón. Se tendrá en cuenta el diseño de la interacción realizada, de modo que sea intuitiva y fácil de usar.

- Se deben incluir los distintos tipos de luces de OpenGL, como direccional, puntual y cono de luz. Al menos se debe poder cambiar la posición y parámetros de una de las luces de un modo interactivo.

- Los objetos deben tener asociado un color, material y textura. Debe realizarse la aplicación de textura con los parámetros adecuados.

- Se debe permitir realizar el sombreado de Gouraud y plano, permitiendo cambiar entre dichos modos.

- Se debe utilizar la función glutIdleFunc() para animar parte de la escena de manera automática.

- No se admitirá ningún proyecto de prácticas que desarrollen prácticas de cursos anteriores. En ese caso el profesor solicitará la subsanación de la propuesta de proyecto.
