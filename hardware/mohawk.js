// title      : BASEline Mohawk
// author     : BASEline
// license    : MIT License
// revision   : 1.0
// tags       : GPS,data,arduino,flysight
// file       : mohawk.jscad

const qty = 1

const thic = 2 // wall thickness
const hawkR = 115
const hawkZ = -93.5 // case circle center
const helmetR = 151
const helmetZ = -140 // helmet circle center

const width = 26
const halfwidth = width / 2

const epsilon = 0.2

function main() {
  return union(
    // mechanism(),
    // translate([-45, -14, 2], cube({size: [90, 28, 1]})), // center guide
    // color("blue", closure())
    color("purple", body())
  )
}

function body() {
  return union(
    difference(
      outer(),
      helmet(),
      hollow(),
      door(-1),
      screw(),
      translate([-200, -200, -400], cube(400)), // bottom half
      translate([-81, -20, -10], rotate([0, -14, 0], cube(40))), // flat end
      translate([42, -20, 0], rotate([0, 14, 0], cube(40))), // usb flat end
      translate([40.99, -6.3, 7.5], rotate([0, 14, 0], cube({size: [6, 9, 3], radius: [0, .6, .6], fn: 15 * qty}))), // usb hole
      translate([42.2, -7.05, 5.7], rotate([0, 14, 0], cube({size: [2, 10.6, 6], radius: [0, 2, 2], fn: 25 * qty}))) // usb access
    ),
    splits()
  )
}

function closure() {
  return difference(
    door(epsilon - 1),
    switchhole(),
    leds()
  )
}

function leds() {
  const hole = cylinder({r: 1.1, h: 4, fn: 30 * qty})
  return union(
    translate([31.5, halfwidth - 3, 12.15], rotate([-90, 13, 0], scale([1.4,1,1], hole)))
    // translate([36.5, halfwidth - 3, 10.8], rotate([-90, 16, 0], scale([1.4,1,1], hole)))
  )
}

function switchhole() {
  return translate([18, halfwidth - 2.4, 13], rotate([0, 11, 0],
    union(
      cube({size: [9, 1, 4], radius: [.2, 0, .2], fn: 15 * qty}), // switch body
      translate([2.5, 0, 0.9], cube({size: [4.2, 4, 2.2]})) // Switch hole
    )
  ))
}

function door(delta) {
  return difference(
    cylinder({r: hawkR - thic - delta, start: [0, halfwidth - thic, hawkZ], end: [0, halfwidth, hawkZ], fn: 140 * qty}), // wall
    helmet(thic + delta),
    screw(),
    translate([-200, -200, -400], cube(400)), // bottom half
    translate([-79 + delta, -20, -10], rotate([0, -14, 0], cube(40))), // flat end
    translate([40 - delta, -20, 0], rotate([0, 14, 0], cube(40))) // usb flat end
  )
}

function outer(delta) {
  delta = delta || 0
  return cylinder({r: hawkR + delta, start: [0, -halfwidth, hawkZ], end: [0, halfwidth, hawkZ], fn: 140 * qty})
}

function helmet(delta) {
  delta = delta || 0
  return cylinder({r: helmetR + delta, start: [0, -halfwidth, helmetZ], end: [0, halfwidth, helmetZ], fn: 140 * qty})
}

function hollow() {
  return difference(
    cylinder({r: hawkR - thic, start: [0, -11, hawkZ], end: [0, 13, hawkZ], fn: 140 * qty}),
    // cylinder({r: 101, start: [0, -11, -86], end: [0, 9, -86], fn: 140 * qty}), // padding
    helmet(thic),
    translate([-76, -20, -2], rotate([0, -14, 0], cube(40))), // flat end
    translate([40, -20, 0], rotate([0, 14, 0], cube(40))) // usb flat end
  )
}

function screw() {
  return translate([-40.7, 9, 9.7], union(
    cylinder({r: 1.4, start: [0, -5, 0], end: [0, 4, 0], fn: 30 * qty})
    // cylinder({r1: 0, r2: 2.7, start: [0, 1.5, 0], end: [0, 4, 0], fn: 30 * qty})
  ))
}

function splits() {
  return union(
    // translate([37, -11, 8], rotate([0, 12, 0], cube({size: [1, 2, 6]}))), // esp support
    // translate([14, -11, 11], rotate([0, 6, 0], cube({size: [1, 2, 9]}))), // esp support
    translate([8.5, -11, 11], rotate([0, 3, 0], cube({size: [1, 12, 10]}))), // front
    translate([-11, -11, 11], rotate([0, -5, 0], cube({size: [1, 12, 9]}))) // back
  )
}

//////////////////////
function mechanism() {
  return union(
    cpu(),
    battery(),
    bn220()
  )
}

function cpu() {
  return color("green", translate([10, -10, 14], rotate([0, 12, 0],
    cube({size: [32, 18, 4.2]})
  ))) // TinyPICO
}

function battery() {
  return color("gray", translate([-37, -10, 9], rotate([0, -12, 0], cube({size: [22, 20, 4.2]}))))
}

function bn220() {
  return translate([-15, -10, 14], union(
    color("blue", translate([0, 0, 0], cube({size: [22, 20, 4.2]}))),
    color("brown", translate([2, 1, 4.4], cube({size: [18, 18, 2], round: true, radius: [1, 1, 0], fn: 15 * qty})))
  ))
}
