TEMPLATE = subdirs
SUBDIRS = qcloud examples

# docs
docs.target = docs
win32:{
    docs.commands = qdoc doc\\qcloud.qdocconf
}else{
    docs.commands = qdoc doc/qcloud.qdocconf
}
docs.depends = FORCE
QMAKE_EXTRA_TARGETS += docs tests
