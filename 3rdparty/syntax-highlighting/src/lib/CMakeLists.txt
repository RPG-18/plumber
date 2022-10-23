add_library(KF5SyntaxHighlighting)

target_sources(KF5SyntaxHighlighting PRIVATE
        abstracthighlighter.cpp
        context.cpp
        contextswitch.cpp
        definitiondownloader.cpp
        highlightingdata.cpp
        foldingregion.cpp
        format.cpp
        htmlhighlighter.cpp
        ansihighlighter.cpp
        keywordlist.cpp
        rule.cpp
        definition.cpp
        repository.cpp
        state.cpp
        syntaxhighlighter.cpp
        theme.cpp
        wildcardmatcher.cpp
        themedata.cpp
        worddelimiters.cpp
        ksyntaxhighlighting_logging.cpp
        ksyntaxhighlighting_logging.h
    $<TARGET_OBJECTS:SyntaxHighlightingData>
)

target_compile_definitions(KF5SyntaxHighlighting PRIVATE QT_USE_QSTRINGBUILDER HAS_SYNTAX_RESOURCE)
set_target_properties(KF5SyntaxHighlighting PROPERTIES
    EXPORT_NAME SyntaxHighlighting
)

target_link_libraries(KF5SyntaxHighlighting
    PUBLIC
        Qt6::Gui
    PRIVATE
        Qt6::Network
)

set(Forwarding_Header_Names
    AbstractHighlighter
    Definition
    DefinitionDownloader
    FoldingRegion
    Format
    Repository
    State
    SyntaxHighlighter
    Theme
    WildcardMatcher
)

target_include_directories(KF5SyntaxHighlighting
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/KSyntaxHighlighting;>"
)