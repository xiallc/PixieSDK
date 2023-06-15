import os
import sys

sys.path.insert(0, os.path.abspath('.'))

# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Pixie SDK'
copyright = '2023, XIA LLC'
author = 'XIA LLC'
version = '3.9'
autosummary_generate = True

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ['sphinxcontrib.rsvgconverter', 'sphinx.ext.intersphinx', 'sphinx.ext.mathjax',
              'sphinx.ext.viewcode', 'sphinx.ext.graphviz', 'sphinx.ext.todo', 'sphinx.ext.autodoc']

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']
pygments_style = 'sphinx'

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
html_css_files = ['css/custom.css']
html_logo = "_static/images/xia_logo_white_no_text.svg"
html_favicon = '_static/images/favicon.ico'
html_theme_options = {
    "logo_only": True,
    "display_version": True,
    "navigation_depth": 2,
    "style_nav_header_background": "#0054a0"
}

# -- Options for LaTeX output ---------------------------------------------
latex_engine = 'pdflatex'
latex_elements = {
    'papersize': 'letterpaper',
    'pointsize': '12pt',
    'figure_align': 'htbp'
}
latex_documents = [
    ('index', 'pixiesdk_docs.tex', 'Pixie SDK Documentation', 'XIA LLC', 'manual'),
]
