import sys
import re


def pp(x):
    """Returns a pretty-print string representation of a number.
       A float number is represented by an integer, if it is whole,
       and up to two decimal places if it isn't
    """
    if isinstance(x, float):
        if x.is_integer():
            return str(int(x))
        else:
            return "{0:.2f}".format(x)
    return str(x)


class Point(object):
    """A point in a two dimensional space"""
    def __init__(self, x, y):
        self.x=float(x)
        self.y=float(y)

    def __repr__(self):
        return '(' + pp(self.x) + ',' + pp(self.y) + ')'


class Line(object):
    """A line between two points"""
    def __init__(self,src,dst):
        self.src=src
        self.dst=dst

    def __repr__(self):
        return str(self.src)+'-->'+str(self.dst)


def point_in(p,list):
    """Whether a point is in a list of points"""
    for i in list:
        if i.x==p.x and i.y==p.y:
            return True
    return False


def onSegment(p,line):  # p:a point,line:a line segment
    """Whether a point is on a line segment"""
    x=p.x
    y=p.y
    x1=line.src.x
    y1=line.src.y
    x2=line.dst.x
    y2=line.dst.y
    dxc = x - x1
    dyc = y - y1
    dxl = x2 - x1
    dyl = y2 - y1
    cross = dxc * dyl - dyc * dxl
    if (abs(cross) < 0.00000001):
        if (abs(dxl) >= abs(dyl)):
            if dxl > 0:
                return (x1 <= x and x <= x2)
            else:
                return (x2 <= x and x <= x1)
            #return dxl > 0 if (point1.x <= p.x and p.x <= point2.x) else (point2.x <= p.x and p.x <= point1.x)
        else:
            if dyl > 0:
                return (y1 <= y and y <= y2)
            else:
                return (y2 <= y and y <= y1)
            #return dyl > 0 if (point1.y <= p.y and p.y <= point2.y) else (point2.y <= p.y and p.y <= point1.y)

    return False


def intersect(l1,l2):
    """Returns a list of points at which two lines intersect"""
    inter=[]  # a list of intersection points
    x1=l1.src.x
    y1=l1.src.y
    x2=l1.dst.x
    y2=l1.dst.y
    x3=l2.src.x
    y3=l2.src.y
    x4=l2.dst.x
    y4=l2.dst.y
    if ((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4))!=0:  # if the two line segments are not parallel
        xoord = ((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4))
        yoord = ((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4))
        if (xoord<=max(x1,x2) and xoord>=min(x1,x2) and yoord<=max(y1,y2) and yoord>=min(y1,y2) and xoord<=max(x3,x4) and xoord>=min(x3,x4) and yoord<=max(y3,y4) and yoord>=min(y3,y4)):
            inter.append(Point(xoord, yoord))
    else:  # if the two line segments are parallel
        if x1*(y2-y3)+x2*(y3-y1)+x3*(y1-y2)==0:  # if the two line segments are on the same line
            xmax = max(x1, x2, x3, x4)
            xmin = min(x1, x2, x3, x4)
            ymax = max(y1, y2, y3, y4)
            ymin = min(y1, y2, y3, y4)
            if (xmax-xmin)<=(abs(x1-x2)+abs(x3-x4)):  # if the two line segments have overlapping parts
                for p in [l1.src, l1.dst, l2.src, l2.dst]:
                    if (p.x < xmax and p.x > xmin) or (p.y < ymax and p.y > ymin):
                        inter.append(p)
    return inter


def parse_line(line):
    """return command, street name and coordinates of street"""
    sp=line.strip().split(" ", 1)  # sp:['command','.....']
    command=sp[0]
    if command=='a' or command=='c':  # add or change
        if len(sp)==1 :  # miss content after a or c command
            raise Exception('Error: wrong input format')
        content = sp[1].strip()
        pos=content.find('"', 1)  # pos is the position of the second quote
        if content[0]!="\"" or content[pos]!="\"" or content[pos+1]!=" ":  #  if the street name is not included between double quotes or there isn't any space after street name
            raise Exception('Error: wrong input format')
        street_name = content[1:pos].lower()
        street_coord_pre = re.findall(r"\((.*?),(.*?)\)", content[pos + 1:])  # street_coord_pre:[(' ',' '),(' ',' '),(' ',' ')], entry of street_coord_pre is tuple

        # decide whether miss bracket or no bracket
        num1 = len(re.findall(r"\(", content[pos + 1:]))
        num2 = len(re.findall(r"\)", content[pos + 1:]))
        if num1 != num2 or num1 == num2 == 0:  # miss parenthesis
            raise Exception('Error: miss bracket')

        street_coord_point = []  # a list of points of street coordinates
        for x in street_coord_pre:
            p = Point(float(x[0]), float(x[1]))
            street_coord_point.append(p)

        line_coord = []  # a list of lines which make up streets
        for i in range(0,len(street_coord_point)-1):
            l = Line(street_coord_point[i], street_coord_point[i + 1])
            line_coord.append(l)

    elif command =='g':  # generate
        if len(sp) != 1:  # there can't be any content after g command
            raise Exception('Error: wrong input')
        street_name=''
        line_coord=[]

    elif command=='r':  # remove
        if len(sp) == 1:  # miss content after r command
            raise Exception('Error: wrong input')
        content = sp[1].strip()
        pos = content.find('"', 1)
        if content[0] != "\"" or content[pos] != "\"" or pos!=len(content)-1:  # if the street name is not included between double quotes
            raise Exception('Error: wrong input')
        street_name = content[1:pos].lower()
        line_coord = []

    else:
        raise Exception('Error: unknown command')  # if the command is not a,c,r or g

    return command, street_name, line_coord


def add(Street,street_name,street_coord):
    Street[street_name]=street_coord


def change(Street,street_name,street_coord):
    Street[street_name] = street_coord


def remove(Street, street_name):
    Street[street_name]=[]


def same_st(list,l1,l2):
    """whether line1 and line2 are in the same street"""
    for l in list:
        if l1 in l and l2 in l:
            return True
    return False


def same_point(p1,p2):
    if p1.x==p2.x and p1.y==p2.y:
        return True
    return False


def reachable(l,vertex):
    """whether there is a point between src and dis on that line segment"""
    for v in vertex:
        if same_point(v,l.src)==True or same_point(v,l.dst)==True:
            continue
        if onSegment(v,l)==True:  # if there exists a point on that line segment
            return False
    return True


def get_key(V,p):
    """given the x,y of a point, return the key of point in dictionary V """
    for k in V.keys():  # V[k]=[ , ]
        if (p.x == V[k][0]) and (p.y == V[k][1]):
            return k


def generate_graph(Street):
    line=[]
    intersection=[]  # a list made up of intersection points
    vertex=[]  # a list made up of vertices

    for i in Street.values():
        for j in i:
            line.append(j)  # line=[l1,l2,l3,l4,l5,l6]

    for i in range(0,len(line)-1):  # every line will be compared with all the lines which are after it
        for j in range(i+1,len(line)):
            if same_st(Street.values(), line[i], line[j])==False:  # if the two line are not on the same street
                p = intersect(line[i], line[j])  # p:[]            # then generate their intersection points
                if p!=[]:  # if there is any intersection points of the two line segments
                    for p1 in p:
                        if point_in(p1, intersection) == False:  # if p1 is not in the intersection list, then put it in the list
                            intersection.append(p1)
                            vertex.append(p1)

    for p in intersection:  # for every intersection point
        for l in line:  # for every line segment of all the streets
            if onSegment(p,l)==True:  # if the intersection point is on that line, then add src and dst points of the line to vertex
                if point_in(l.src, vertex) == False:
                    vertex.append(l.src)
                if point_in(l.dst, vertex) == False:
                    vertex.append(l.dst)  # vertex=[p1,p2,p3...]

    V={}
    index_v=1
    for v in vertex: # v:point
        V[index_v] = [v.x, v.y]  # convert the list of vertices to a dictionary
        index_v+=1

    # generate E
    E=[]
    edge=[]
    for i in intersection:
        for v in vertex:
            if (i.x==v.x and i.y==v.y)==False:  # if i and v are not the same point
                for l in line:
                    if onSegment(i, l) == True and onSegment(v, l) == True:  # if intersection point i and vertex v are both on that line
                        if reachable(Line(i, v), vertex) == True:  # and if one is reachable from the other without traversing another vertex
                            edge.append(Line(i, v))  # then Line(i,v) is an edge

    for e in edge:  # get the index of edge's src and dst point in V
        k1= get_key(V, e.src)
        k2= get_key(V, e.dst)
        if [k1,k2] not in E and [k2,k1] not in E:
            E.append([k1,k2])

    return V, E


def main():
    ### YOUR MAIN CODE GOES HERE

    ### sample code to read from stdin.
    ### make sure to remove all spurious print statements as required
    ### by the assignment

    Street = {}
    V = {}
    E = []
    street_name_record=''

    while True:
        line = sys.stdin.readline()
        if line == '':
            break
        # print 'read a line:', line

        try:
            command, street_name, street_coord = parse_line(line)
            if command == 'a':
                if street_name_record==street_name:  # if a street has already been added
                    raise Exception('Error: cannot add a street twice')
                street_name_record=street_name
                add(Street, street_name, street_coord)
            if command == 'c':
                if street_name not in Street:  # we can only change a street that has already been added
                    raise Exception('Error: change a street that does not exist')
                change(Street, street_name, street_coord)
            if command == 'r':
                if street_name not in Street:  # we can only remove a street that has already been added
                    raise Exception('Error: remove a street that does not exist')
                remove(Street, street_name)
            if command == 'g':
                V, E = generate_graph(Street)

                # print V and E
                print 'V = {'
                for k in V.keys():
                    print  str(k)+': ('+pp(V[k][0])+','+pp(V[k][1])+')'
                print '}'

                print 'E = {'
                for e in range(0, len(E)):
                    if e != len(E) - 1:
                        print '<' + str(E[e][0]) + ',' + str(E[e][1]) + '>' + ','
                    else:
                        print '<' + str(E[e][0]) + ',' + str(E[e][1]) + '>'
                print '}'

        except Exception as ex:
            sys.stderr.write(str(ex) + '\n')

    # print 'Finished reading input'

    # return exit code 0 on successful termination
    sys.exit(0)

if __name__ == '__main__':
    main()
