# BASEline Mohawk GPS

## 3D printed plastic case for Mohawk

This directory contains designs for a 3D printed case.
The case is made of two parts, a main body and door which is held on by an M3 machine screw.

## Generate STL file for 3D printing

The design was created using [JSCAD](https://github.com/jscad/OpenJSCAD.org).
JSCAD is a tool for generating 3D CAD designs from JavaScript code.
This is nice because it is fully open source and does not depend on any proprietary software to generate 3D CAD files for printing.

The source for the design is in`mohawk.js`.

To generate the case, run:

```bash
npm install
npm run mohawk
```

## Printing

Use any 3D printing software to print the generated `mohawk.stl` file.

If you don't have access to a printer, there are also online services like [Shapeways](https://www.shapeways.com/) which do 3D printing on-demand.
