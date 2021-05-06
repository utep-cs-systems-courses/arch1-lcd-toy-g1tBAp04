#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <Circlelib>

const AbRect rect10 = {abRectGetBounds, abRectCheck, 10,10};;

//square code
void
abDrawPos(AbShape *shape, Vec2 *shapeCenter, u_int fg_color, u_int bg_color)
{
  u_char row, col;
  Region bounds;
  abShapeGetBounds(shape, shapeCenter, &bounds);
  lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1],
	      bounds.botRight.axes[0]-1, bounds.botRight.axes[1]-1);
  for (row = bounds.topLeft.axes[1]; row < bounds.botRight.axes[1]; row++) {
    for (col = bounds.topLeft.axes[0]; col < bounds.botRight.axes[0]; col++) {
      Vec2 pixelPos = {col, row};
      int color = abShapeCheck(shape, shapeCenter, &pixelPos) ?
	fg_color : bg_color;
      lcd_writeColor(color);
    }
  }
}

//circle code
int abCircleCheck(const AbCircle *circle, const Vec2 *centerPos, const Vec2 *pixel)
{
  u_char radius = circle->radius;
  int axis;
  Vec2 relPos;
  vec2Sub(&relPos, pixel, centerPos); /* vector from center to pixel */
  vec2Abs(&relPos);		      /* project to first quadrant */
  return (relPos.axes[0] <= radius && circle->chords[relPos.axes[0]] >= relPos.axes[1]);
}
  
void
abCircleGetBounds(const AbCircle *circle, const Vec2 *centerPos, Region *bounds)
{
  u_char axis, radius = circle->radius;
  for (axis = 0; axis < 2; axis ++) {
    bounds->topLeft.axes[axis] = centerPos->axes[axis] - radius;
    bounds->botRight.axes[axis] = centerPos->axes[axis] + radius;
  }
  regionClipScreen(bounds);
}
