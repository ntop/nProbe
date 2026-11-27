![nprobe_logo]
# nProbe
This repository contains open source components and extensions for nProbe available at http://www.ntop.org/products/netflow/nprobe/

Here you can find:
* Add-on and extensions
* Code examples of applications that can interact with nProbe

Enjoy!

[nprobe_logo]: http://www.ntop.org/images/2025/06/nprobe-icon.webp

### Documentation

If you want to learn more about nProbe please visit the [User's Guide](https://www.ntop.org/guides/nprobe/).



### Building the User's Guide

The User's Guide is a [VitePress](https://vitepress.dev) project in `doc/src/` (Markdown pages, same
URLs as the old Sphinx guide: `/guides/nprobe/<page>.html`). Requires Node 18+ and `make`.

```
cd doc/src
make dist      # build the static site into doc/src/dist (runs npm install if needed)
make serve     # build, then serve it at http://localhost:4173/guides/nprobe/   (PORT=8080 make serve)
```

For live reload while editing a page use `npm run dev` (http://localhost:5173/guides/nprobe/).

### Details
For more information about nProbe, please visit [http://ntop.org](https://www.ntop.org/products/netflow/nprobe/).
