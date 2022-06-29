/**
 * BASEline Mohawk
 * 3D printing CAD design for Mohawk GPS case written in JSCAD
 */

const jscad = require("@jscad/modeling")
const { subtract, union } = jscad.booleans
const { colorize } = jscad.colors
const { extrudeLinear } = jscad.extrusions
const { cube, cuboid, cylinder, roundedRectangle } = jscad.primitives
const { rotate, rotateX, rotateY, scale, translate } = jscad.transforms

const qty = 1

const thic = 2 // wall thickness
const hawkR = 115
const hawkZ = -93.5 // case circle center
const helmetR = 151
const helmetZ = -140 // helmet circle center

const width = 26
const halfwidth = width / 2

const epsilon = 0.2

const rad = (deg) => deg * Math.PI / 180

const cylinderY = (opt) => rotateX(Math.PI / 2, cylinder(opt))

function main() {
  return [
    // mechanism(),
    // cuboid({ size: [90, 28, 1] }), // center guide
    // colorize([0, 0, 1], closure()),
    colorize([0.4, 0.2, 0.55], body())
  ]
}

function body() {
  return union(
    subtract(
      outer(),
      helmet(),
      hollow(),
      door(-1),
      screw(),
      cube({ size: 400, center: [0, 0, -200] }), // bottom half
      translate([-63, 0, 0], rotateY(rad(-14), cube({ size: 40 }))), // flat end
      translate([62.5, 0, 0], rotateY(rad(14), cube({ size: 40 }))), // usb flat end
      usbHole()
    ),
    splits()
  )
}

function closure() {
  return subtract(
    door(epsilon - 1),
    screw(),
    switchhole(),
    leds()
  )
}

function usbHole() {
  const hole = extrudeLinear({ height: 2 }, roundedRectangle({ size: [3, 9], roundRadius: 0.6, segments: 15 * qty }))
  const recess = extrudeLinear({ height: 8 }, roundedRectangle({ size: [6.2, 11], roundRadius: 1, segments: 25 * qty }))
  return [
    translate([42, -1.9, 9], rotateY(rad(104), hole)), // usb hole
    translate([43, -1.9, 8.6], rotateY(rad(104), recess)) // usb recess
  ]
}

function leds() {
  const hole = cylinderY({ radius: 1.1, height: 6, segments: 30 * qty })
  return [
    translate([31.5, halfwidth - 3, 12.15], rotateY(rad(13), scale([1.4, 1, 1], hole)))
    // translate([36.5, halfwidth - 3, 10.8], rotateY(rad(16), scale([1.4, 1, 1], hole)))
  ]
}

function switchhole() {
  const recess = extrudeLinear({ height: 0.4 }, roundedRectangle({ size: [9, 4], roundRadius: 0.2, segments: 15 * qty }))
  return translate([22.8, halfwidth - 2, 14.1], rotateY(rad(11),
    [
      rotateX(rad(-90), recess), // switch recess
      cuboid({ size: [4.2, 4, 2.2] }) // switch hole
    ]
  ))
}

function door(delta) {
  return subtract(
    translate([0, halfwidth - thic / 2, hawkZ], cylinderY({ radius: hawkR - thic - delta, height: thic, segments: 140 * qty })), // wall
    helmet(thic + delta),
    // cube({ size: 400, center: [0, 0, -200] }), // bottom half
    translate([-61 + delta, 0, 0], rotateY(rad(-14), cube({ size: 40 }))), // flat end
    translate([60.5 - delta, 0, 0], rotateY(rad(14), cube({ size: 40 }))) // usb flat end
  )
}

function outer(delta) {
  delta = delta || 0
  return translate([0, 0, hawkZ], cylinderY({ radius: hawkR + delta, height: width, segments: 140 * qty }))
}

function helmet(delta) {
  delta = delta || 0
  return translate([0, 0, helmetZ], cylinderY({ radius: helmetR + delta, height: width, segments: 140 * qty }))
}

function hollow() {
  return subtract(
    translate([0, 1, hawkZ], cylinderY({ radius: hawkR - thic, height: 24, segments: 140 * qty })),
    // translate([0, -1, -86], cylinderY({ radius: 101, height: 20, segments: 140 * qty })), // padding
    helmet(thic),
    translate([-55.5, 0, -2], rotateY(rad(-14), cube({ size: 40 }))), // flat end
    translate([60.5, 0, 0], rotateY(rad(14), cube({ size: 40 }))) // usb flat end
  )
}

function screw() {
  return translate([-40.7, 9, 9.7], cylinderY({ radius: 1.4, height: 10, segments: 30 * qty }))
}

function splits() {
  return [
    // translate([38, -10, 11], rotateY(rad(12), cuboid({ size: [1, 2, 6] }))), // esp support
    // translate([15, -10, 16], rotateY(rad(6), cuboid({ size: [1, 2, 9] }))), // esp support
    translate([9.3, -5, 16], rotateY(rad(3), cuboid({ size: [1, 12, 10] }))), // front
    translate([-11, -5, 16], rotateY(rad(-5), cuboid({ size: [1, 12, 9] }))) // back
  ]
}

//////////////////////
function mechanism() {
  return [
    tinyPico(),
    battery(),
    bn180()
  ]
}

function tinyPico() {
  return colorize([0, 0.5, 0], translate([26, -1, 13], rotateY(rad(12),
    cuboid({ size: [32, 18, 4.2] })
  )))
}

function battery() {
  return colorize([0.5, 0.5, 0.5], translate([-24, 0, 14], rotateY(rad(-12),
    cuboid({ size: [22, 20, 4.2] })
  )))
}

function bn180() {
  const antenna = extrudeLinear({ height: 2 },
    roundedRectangle({ size: [18, 18], roundRadius: 1, segments: 15 * qty })
  )
  return translate([-1, 0, 16], [
    colorize([0, 0, 0.8], cuboid({ size: [18, 18, 1] })),
    colorize([0.8, 0.8, 0.8], cuboid({ size: [18, 12, 2], center: [0, -3, -1.7] })),
    colorize([0.5, 0.2, 0.2], translate([0, 0, 0.6], antenna))
  ])
}

module.exports = { main }
