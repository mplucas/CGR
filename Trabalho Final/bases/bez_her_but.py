import pygame
from pygame.locals import *
from math import pow, sqrt


class vec2d(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y


# COLORS
# Stuff
gray = (100, 100, 100)
lightgray = (200, 200, 200)
red = (255, 0, 0)
white = (255, 255, 255)
black = (0, 0, 0)
# Buttons
pink_btNotC = (255, 204, 204)
pink_btSelect = (255, 80, 80)
green_btC0 = (204, 255, 204)
green_btC1 = (153, 255, 153)
green_btC2 = (102, 255, 102)
green_btSelect = (51, 204, 51)
# Points
green = (0, 255, 0)
blue = (0, 0, 255)
yellow = (255, 255, 0)
# Lines
yellow_line = (255, 255, 204)
blue_line = (204, 255, 255)
# Curves
yellow_curve = (255, 204, 0)
blue_curve = (51, 204, 255)

X, Y, Z = 0, 1, 2

# Dimensions
axis_x = 1366
axis_y = 768
screen = pygame.display.set_mode((axis_x, axis_y))
action = - 1


def text_objects(text, font):
    textSurface = font.render(text, True, black)
    return textSurface, textSurface.get_rect()


def set_action(a):
    global action
    action = a


def button(string, x, y, width, height, inactiveClr, activeClr, act=None):
    mouse = pygame.mouse.get_pos()
    click = pygame.mouse.get_pressed()
    if x + width > mouse[0] > x and y + height > mouse[1] > y:
        pygame.draw.rect(screen, activeClr, (x, y, width, height))
        if click[0] == 1 and act is not None:
            set_action(act)
    else:
        pygame.draw.rect(screen, inactiveClr, (x, y, width, height))
    smallText = pygame.font.Font("freesansbold.ttf", 20)
    textSurf, textRect = text_objects(string, smallText)
    textRect.center = ((x + (width / 2)), (y + (height / 2)))
    screen.blit(textSurf, textRect)


def magnitude_vec(v):
    return sqrt((v.x * v.x) + (v.y * v.y))


def produto_escalar(v1, v2):
    return (v1.x * v2.x) + (v1.y * v2.y)


def projecao(v1, v2):
    k = produto_escalar(v1, v2) / produto_escalar(v2, v2)
    return vec2d(v2.x * k, v2.y * k)


def sub_vec(v1, v2):
    return vec2d(v1.x - v2.x, v1.y - v2.y)


def sum_vec(v1, v2):
    return vec2d(v1.x + v2.x, v1.y + v2.y)


def versor(v):
    mag = magnitude_vec(v)
    return vec2d(v.x / mag, v.y / mag)


def mult_escalar(v, k):
    return vec2d(v.x * k, v.y * k)


def alinha_vec(v0, v0u):
    nv = sub_vec(v0u, v0)
    return sum_vec(nv, v0u)


def compute_bezier_points(vertices):
    result = []

    p0x = vertices[0][0]
    p0y = vertices[0][1]
    p1x = vertices[1][0]
    p1y = vertices[1][1]
    p2x = vertices[2][0]
    p2y = vertices[2][1]
    p3x = vertices[3][0]
    p3y = vertices[3][1]
    p4x = vertices[4][0]
    p4y = vertices[4][1]
    p5x = vertices[5][0]
    p5y = vertices[5][1]

    t = 0
    step = 0.01

    while t <= (1 + step):
        pointX = pow(1 - t, 5) * p0x + \
                 5 * t * pow(1 - t, 4) * p1x + \
                 10 * t * t * pow(1 - t, 3) * p2x + \
                 10 * t * t * t * pow(1 - t, 2) * p3x + \
                 5 * t * t * t * t * (1 - t) * p4x + \
                 t * t * t * t * t * p5x
        pointY = pow(1 - t, 5) * p0y + \
                 5 * t * pow(1 - t, 4) * p1y + \
                 10 * t * t * pow(1 - t, 3) * p2y + \
                 10 * t * t * t * pow(1 - t, 2) * p3y + \
                 5 * t * t * t * t * (1 - t) * p4y + \
                 t * t * t * t * t * p5y

        result.append((int(pointX), int(pointY)))
        t += step

    return result


def compute_hermit_points(vertices):
    result = []

    p0x = vertices[0][0]
    p0y = vertices[0][1]
    p1x = vertices[2][0]
    p1y = vertices[2][1]
    p0ux = vertices[1][0] - p0x
    p0uy = vertices[1][1] - p0y
    p1ux = vertices[3][0] - p1x
    p1uy = vertices[3][1] - p1y

    t = 0
    step = 0.01

    while t <= (1 + step):
        pointX = ((2 * t * t * t) - (3 * t * t) + 1) * p0x + \
                 ((-2 * t * t * t) + (3 * t * t)) * p1x + \
                 ((t * t * t) - (2 * t * t) + t) * p0ux + \
                 ((t * t * t) - (t * t)) * p1ux

        pointY = ((2 * t * t * t) - (3 * t * t) + 1) * p0y + \
                 ((-2 * t * t * t) + (3 * t * t)) * p1y + \
                 ((t * t * t) - (2 * t * t) + t) * p0uy + \
                 ((t * t * t) - (t * t)) * p1uy

        result.append((int(pointX), int(pointY)))
        t += step

    return result


def interpolate_curves(curve1, curve2):
    v = sub_vec(curve1[len(curve1) - 1], curve2[0])
    for i in range(0, len(curve2)):
        curve2[i].x += v.x
        curve2[i].y += v.y
    return curve2


def mag_derivate_x_y(b_curve):
    p4x = b_curve[4][0]
    p4y = b_curve[4][1]
    p5x = b_curve[5][0]
    p5y = b_curve[5][1]
    dx = 5 * p5x - 5 * p4x
    dy = 5 * p5y - 5 * p4y
    return magnitude_vec(vec2d(dx, dy))


def generate_pRetaTg(b_curve):
    p4x = b_curve[4][0]
    p4y = b_curve[4][1]
    p5x = b_curve[5][0]
    p5y = b_curve[5][1]
    dx = 5 * p5x - 5 * p4x
    dy = 5 * p5y - 5 * p4y
    if p5x > p4x:
        k = 50
    else:
        k = -50
    if dx == 0:
        if p5y > p4y:
            p_retaTg = vec2d(0, -k)
        else:
            p_retaTg = vec2d(0, k)
    else:
        m = dy / dx
        p_retaTg = vec2d(k, k * m)

    return sum_vec(p_retaTg, vec2d(p5x, p5y))


def point_C1(b_curve):
    pointTg = generate_pRetaTg([(x.x, x.y) for x in b_curve])
    return sum_vec(mult_escalar(versor(sub_vec(pointTg, b_curve[5])),
                                mag_derivate_x_y([(x.x, x.y) for x in b_curve])), b_curve[5])


def main():
    pygame.init()

    # The currently selected point
    selected = None

    # Control points that are later used to calculate the curve
    b_control_points = [vec2d(100, 350), vec2d(200, 350), vec2d(300, 350),
                        vec2d(400, 350), vec2d(500, 350), vec2d(600, 350)]
    h_control_points = [vec2d(700, 350), vec2d(800, 350), vec2d(900, 350), vec2d(1000, 350)]

    # Flip screen
    pygame.display.flip()
    clock = pygame.time.Clock()

    running = True
    while running:
        for event in pygame.event.get():
            if event.type in (QUIT, KEYDOWN):
                running = False
            elif event.type == MOUSEBUTTONDOWN and event.button == 1:
                for p in b_control_points:
                    if abs(p.x - event.pos[X]) < 10 and abs(p.y - event.pos[Y]) < 10:
                        selected = p
                for p in h_control_points:
                    if abs(p.x - event.pos[X]) < 10 and abs(p.y - event.pos[Y]) < 10:
                        selected = p
            elif event.type == MOUSEBUTTONUP and event.button == 1:
                selected = None

        # Draw stuff
        screen.fill(gray)

        if selected is not None:
            selected.x, selected.y = pygame.mouse.get_pos()
            pygame.draw.circle(screen, white, (selected.x, selected.y), 10)

        # Draw control points
        for p in b_control_points:
            pygame.draw.circle(screen, blue, (int(p.x), int(p.y)), 4)
        for p in h_control_points:
            pygame.draw.circle(screen, yellow, (int(p.x), int(p.y)), 4)
        if b_control_points[5].x == h_control_points[0].x and b_control_points[5].y == h_control_points[0].y:
            pygame.draw.circle(screen, green, (int(h_control_points[0].x), int(h_control_points[0].y)), 4)

        # Draw control "lines"
        pygame.draw.lines(screen, blue_line, False, [(x.x, x.y) for x in b_control_points])
        pygame.draw.line(screen, lightgray, (h_control_points[0].x, h_control_points[0].y),
                         (h_control_points[1].x, h_control_points[1].y))
        pygame.draw.line(screen, lightgray, (h_control_points[2].x, h_control_points[2].y),
                         (h_control_points[3].x, h_control_points[3].y))

        # Draw bezier and hermite curves
        b_points = compute_bezier_points([(x.x, x.y) for x in b_control_points])
        pygame.draw.lines(screen, blue_curve, False, b_points, 2)
        h_points = compute_hermit_points([(x.x, x.y) for x in h_control_points])
        pygame.draw.lines(screen, yellow_curve, False, h_points, 2)

        # Draw buttons
        button("C0", 75, 50, 85, 40, green_btC0, green_btSelect, 0)
        button("C1", 190, 50, 85, 40, green_btC1, green_btSelect, 1)
        button("C2", 305, 50, 85, 40, green_btC2, green_btSelect, 2)
        button("Restart", 420, 50, 85, 40, pink_btNotC, pink_btSelect, 3)

        if action == 0:
            h_control_points = interpolate_curves(b_control_points, h_control_points)
            set_action(-1)
        elif action == 1:
            h_control_points = interpolate_curves(b_control_points, h_control_points)
            h_control_points[1] = point_C1(b_control_points)
            set_action(-1)
        elif action == 3:
            b_control_points = [vec2d(100, 350), vec2d(200, 350), vec2d(300, 350),
                                vec2d(400, 350), vec2d(500, 350), vec2d(600, 350)]
            h_control_points = [vec2d(700, 350), vec2d(800, 350), vec2d(900, 350), vec2d(1000, 350)]
            set_action(-1)

        # Flip screen
        pygame.display.flip()
        clock.tick(100)
        # print clock.get_fps()


if __name__ == '__main__':
    main()
