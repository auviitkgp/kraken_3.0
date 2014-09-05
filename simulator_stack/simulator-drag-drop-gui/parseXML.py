def parseSimX():

    posDict = {}

    import xml.etree.ElementTree as ET
    tree = ET.parse('scene_1.xml')
    root = tree.getroot()

    for joint in root.iter('joint'):

        attribs = joint.attrib

        nameString = attribs['name']

        for i in range(3):

            if joint[i].tag == 'origin':

                pos = joint[i].attrib['xyz']

                # print pos
                # print type(pos)
                # print pos.split(' ')

                possplt = pos.split(' ')

                t = [float(possplt[i]) for i in range(len(possplt))]

                # print t

                posDict[nameString] = t[:]

                break

    return posDict