set (TE_SIMPLESCENE_INC_NOFILTER
    "TeApplication.h"
)

set (TE_SIMPLESCENE_SRC_NOFILTER
    "Main.cpp"
    "TeApplication.cpp"
)

source_group ("" FILES ${TE_SIMPLESCENE_SRC_NOFILTER} ${TE_SIMPLESCENE_INC_NOFILTER})

set (TE_SIMPLESCENE_SRC
    ${TE_SIMPLESCENE_INC_NOFILTER}
    ${TE_SIMPLESCENE_SRC_NOFILTER}
)
