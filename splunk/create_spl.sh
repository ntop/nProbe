 #!/bin/bash         
 tar -cvzf nprobe.tar.gz nprobe/
 mv nprobe.tar.gz nprobe.spl

 # On Mac OS X, use gnutar rather than the default tar packaged with the OS. The default tar utility generates a series of warnings that can be problematic when packaging your app.