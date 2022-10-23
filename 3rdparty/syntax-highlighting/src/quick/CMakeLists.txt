# SPDX-FileCopyrightText: 2018 Eike Hein <hein@kde.org>
# SPDX-FileCopyrightText: 2021 Volker Krause <vkrause@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_library(kquicksyntaxhighlightingplugin)
target_sources(kquicksyntaxhighlightingplugin PRIVATE
    kquicksyntaxhighlightingplugin.cpp
    kquicksyntaxhighlighter.cpp
    repositorywrapper.cpp
)

target_link_libraries(kquicksyntaxhighlightingplugin PRIVATE
    KF5SyntaxHighlighting
    Qt${QT_MAJOR_VERSION}::Quick
)

target_compile_definitions(
        kquicksyntaxhighlightingplugin
        PRIVATE
        KSYNTAXHIGHLIGHTING_STATIC_DEFINE
)