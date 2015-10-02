def getVerboseTag(argv):
    verboseTags = ['--verbose', '--debug']
    for i in argv:
        if i in verboseTags:
            return True

    return False
