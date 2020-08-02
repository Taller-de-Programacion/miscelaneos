# Packaging en linux: construir un paquete .deb

Vamos a ver los pasos para crear un paquete de instalación para sistemas debian.

Para esto necesitamos

* cmake

* fakeroot

* dpkg-deb

* lintian (opcional)

## El proyecto.

Nuestro proyecto es un cliente socket cuya fuente está organizada en:

* Una carpeta `src` con el código fuente.

* Una carpeta `include` con los headers de nuestras fuentes.

* Una carpeta `ui` con los archivos `.ui` del framework Qt.

* Un archivo de compilación `CMakeLists.txt`

* Una carpeta `package` con archivos necesarios para armar el paquete debian


## Compilación del proyecto

Creamos una carpeta en donde realizar la compilación. Por convención la llamamos `build`, puede tener el nombre que el usuario prefiera. Ingresamos a la carpeta y ejecutamos `cmake ..` para crear el *makefile* y `make` para compilar. Esto nos va a generar un ejecutable `test-client`.

Si lo ejecutamos podemos ver que se abre una interfaz gráfica donde podemos ingresar un *host* y un *puerto* y conectarnos a un servidor (por ejemplo un netcat), e imprimir los mensajes que vamos recibiendo

## Armado del paquete

Para armar un paquete .deb necesitamos un archivo *control* dentro de una carpeta **DEBIAN**. Este archivo tiene descripción del paquete a armar, como la arquitectura, categoría del paquete, dependencias, etc.

El script que utilizamos realiza lo siguiente:

* Armo estructura de directorios en una carpeta con el nombre del paquete a armar. Creo una carpeta con el nombre `$PACKAGE_NAME`, donde esta variable es el nombre que tendrá nuestro paquete de instalación. Colocamos los archivos como si fuera `$PACKAGE_NAME` fuera la raiz del sistema en donde instalamos nuestro paquete.

~~~{.bash}
mkdir -p $PACKAGE_NAME/usr/bin/
mkdir -p $PACKAGE_NAME/usr/share/applications/
mkdir -p $PACKAGE_NAME/usr/share/pixmaps/
~~~

* Copio el archivo de control, en la carpeta DEBIAN, con la configuración de mi paquete.

~~~{.bash}
cp -r DEBIAN $PACKAGE_NAME/
~~~

* Los archivos .desktop nos ayudan a configurar una entrada en el menú "inicio" de los gestores de ventana (KDE, Gnome, etc)

~~~{.bash}
cp assets/$APP_NAME.desktop $PACKAGE_NAME/usr/share/applications/
# Copio logo
cp assets/logo.png $PACKAGE_NAME/usr/share/pixmaps/$APP_NAME.png
~~~

* Copio el ejecutable, le saco símbolos de debug y código muerto para minimizar su tamaño.

~~~{.bash}
objcopy --strip-debug --strip-unneeded $BUILT_APP $PACKAGE_NAME/usr/bin/$APP_NAME
~~~

* Le configuro los permisos de sólo lectura a los archivos. Al logo le saco permiso de ejecución.

~~~{.bash}
chmod -R 755 $PACKAGE_NAME
chmod -x $PACKAGE_NAME/usr/share/pixmaps/$APP_NAME.png
~~~

* Armo el paquete simulando ser root

~~~{.bash}
fakeroot dpkg-deb --build $PACKAGE_NAME
~~~

# Probando el paquete con Docker

Este paso es opcional y bastante recomendable.

Docker es un servicio que nos permite crear *containers* para correr nuestras aplicaciones en ambientes aislados, similar a *chroot* pero con una interfaz más amigable al usuario.

En un archivo `Dockerfile` elegimos la imagen base sobre la que queremos trabajar. En mi caso elegí `FROM ubuntu` por comodidad. Luego copiamos el paquete debian que queremos correr, y ponemos un comando por cada `RUN`, en nuestro caso concatenamos comandos para ejecutarlos dentro de una misma capa.

Por ejemplo

~~~
apt update && \
apt install -y cowsay && \
dpkg -i /tmp/test-client_0.0.1.deb
~~~

Primero actualiza los repositorios, luego instala la aplicación *cowsay* y luego instala nuestro paquete debian. (Nota: `cowsay` luego deberá ser cambiado para instalar las dependencias reales de nuestra aplicación)

~~~{.bash}
docker build -t test-client .
~~~

Donde `-t test-client` es el *tag* de la imagen que vamos a crear, y `.` es la carpeta que contiene nuestro Dockerfile

(**NOTA**: depende de como tengan configurada la instalación de docker, es posible que necesiten correr los comandos con "sudo")

## Ejecutando la aplicación en Docker

Una vez que armarmos la imagen, corremos una instancia con:

~~~{.bash}
docker run --rm test-client
~~~

Esto seguramente nos arroje el siguiente error

~~~
qt.qpa.xcb: could not connect to display 
qt.qpa.plugin: Could not load the Qt platform plugin "xcb" in "" even though it was found.
This application failed to start because no Qt platform plugin could be initialized. Reinstalling the application may fix this problem.

Available platform plugins are: eglfs, linuxfb, minimal, minimalegl, offscreen, vnc, xcb.
~~~

Para esto, le pasamos un par de parámetros extra a docker para que pueda usar el servidor gráfico de la máquina host

~~~{.bash}
docker run --rm --env DISPLAY=$DISPLAY --volume $XAUTHORITY:/root/.Xauthority --net=host test-client
~~~

# Bibliografía

* https://www.debian.org/doc/manuals/developers-reference/tools.html

* https://ubuntuforums.org/showthread.php?t=910717

* http://fabiorehm.com/blog/2014/09/11/running-gui-apps-with-docker/
