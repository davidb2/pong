#!/usr/bin/env python3.6
import argparse
import pygame
import re
import sys
import time

POSITION_PATTERN = re.compile(r'x: (-?\d+.\d+), y: (-?\d+.\d+)')

def main(args):
  pygame.init()

  size = (args.width, args.height)
  screen = pygame.display.set_mode(size)
  clock = pygame.time.Clock()
  done = False

  for line in sys.stdin:
    if done:
      break
    for event in pygame.event.get():
      if event.type == pygame.QUIT:
        done = True
    m = re.search(POSITION_PATTERN, line)
    if m:
      x = int((args.width / 2.) * float(m.group(1)) + (args.width / 2.))
      y = int((args.height / 2.) * float(m.group(2)) + (args.height / 2.))

      print(x, y)

      screen.fill((0xff, 0xff, 0xff))
      pygame.draw.circle(screen, (0x00, 0x00, 0x00), (x, y), 10)
      pygame.display.flip()
      clock.tick(args.speed)

if __name__ == '__main__':
  parser = argparse.ArgumentParser('View pong ball')
  parser.add_argument('--width', type=int, default=400)
  parser.add_argument('--height', type=int, default=400)
  parser.add_argument('--speed', type=float, default=60, help='updates per second')
  args = parser.parse_args()
  main(args)

