set (TE_OBJECTIMPORTER_INC_NOFILTER
    "TeObjectImporterPrerequisites.h"
    "TeObjectImporter.h"
    "TeObjectImportData.h"
)

set (TE_OBJECTIMPORTER_SRC_NOFILTER
    "TeObjectImporter.cpp"
    "TeObjectImporterPlugin.cpp"
    "TeObjectImportData.cpp"
)

source_group ("" FILES ${TE_OBJECTIMPORTER_SRC_NOFILTER} ${TE_OBJECTIMPORTER_INC_NOFILTER})

set (TE_OBJECTIMPORTER_SRC
    ${TE_OBJECTIMPORTER_INC_NOFILTER}
    ${TE_OBJECTIMPORTER_SRC_NOFILTER}
)
