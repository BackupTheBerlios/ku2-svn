\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{rtu-thesis}[2010/12/10 RTU thesis and course work class]

% Required packages:
\RequirePackage{ifthen}
\RequirePackage[xetex]{graphicx}
\RequirePackage[a4paper,verbose]{geometry}
\RequirePackage{fontspec}% provides font selecting commands
\RequirePackage{xunicode}% provides unicode character macros
\RequirePackage{xltxtra} % provides some fixes/extras
\RequirePackage{amsmath}
%\RequirePackage{unicode-math}
\RequirePackage{fancyvrb}
\RequirePackage{relsize}
\RequirePackage{xtab}
%\RequirePackage{hyphenat}
\RequirePackage{algpseudocode}

% Font functions:
\input{size12.clo}
\defaultfontfeatures{Mapping=tex-text}
\setmainfont{Times New Roman}
\setmonofont{Courier New}
%\setsansfont{Times New Roman}
%\setmathfont{Times New Roman}
%\setmathrm{Times New Roman}
%\setboldmathrm{Times New Roman}
\newcommand{\bold}{\bfseries}
\newcommand{\inbold}[1]{{\bold #1}}
\newcommand{\italic}{\itshape}
\newcommand{\bolditalic}{\bfseries\itshape}
\newcommand{\fontsizeZ}[2]{\fontsize{#1}{#2}\selectfont}
\newcommand{\sized}[1]{\fontsize{#1}{\f@baselineskip}}
\newcommand{\sizedZ}[1]{\fontsize{#1}{\f@baselineskip}\selectfont}

% Setting paper/page parameters:
\geometry{hdivide={2.5cm,*,1.5cm},vdivide={2cm,*,2cm}}
\pagenumbering{arabic}
\pagestyle{plain}

% Options:
\newcommand{\@OPTworktype}{}
\newcommand{\@OPTthesis}{}
\newcommand{\@OPTpagenumperpart}{}

% Option declarations:
\DeclareOption{draft}{\setlength\overfullrule{5pt}}
\DeclareOption{final}{\setlength\overfullrule{0pt}}
\DeclareOption{leqno}{\input{leqno.clo}}
\DeclareOption{fleqn}{\input{fleqn.clo}}
\DeclareOption{course}{\renewcommand{\@OPTworktype}{course}}
\DeclareOption{lab}{\renewcommand{\@OPTworktype}{lab}}
\DeclareOption{bach-thesis-report}{
	\renewcommand{\@OPTworktype}{bach-thesis-report}
	\renewcommand{\@OPTthesis}{bachelor}}
\DeclareOption{bach-thesis}{
	\renewcommand{\@OPTworktype}{bach-thesis}
	\renewcommand{\@OPTthesis}{bachelor}}
\DeclareOption{master-thesis-report}{
	\renewcommand{\@OPTworktype}{master-thesis-report}
	\renewcommand{\@OPTthesis}{master}}
\DeclareOption{master-thesis}{
	\renewcommand{\@OPTworktype}{master-thesis}
	\renewcommand{\@OPTthesis}{master}}
\DeclareOption{pagenumperpart}{\renewcommand{\@OPTpagenumperpart}{true}}
\ExecuteOptions{draft, master-thesis}
\ProcessOptions

% Text parameters:
\renewcommand{\baselinestretch}{1.5} % line spacing: x1.5
\newcommand{\normalparskip}{\setlength{\parskip}{0.21cm}}
\newcommand{\smallparskip}{\setlength{\parskip}{-0.15cm}}

% Global variables:
\gdef\@university{Rīgas Tehniskā universitāte}
\gdef\@faculty{Datorzinātnes un informācijas tehnoloģijas fakultāte}
\gdef\@institute{Lietišķo datorsistēmu institūts}
\gdef\@department{Sistēmu teorijas un projektēšanas katedra}

\ifthenelse{\equal{\@OPTworktype}{course}}{
	\gdef\@worktype{Studiju darbs}}{
\ifthenelse{\equal{\@OPTworktype}{lab}}{
	\gdef\@worktype{Laboratorijas darbs}}{
\ifthenelse{\equal{\@OPTworktype}{bach-thesis-report}}{
	\gdef\@worktype{Bakalaura darba atskaite}}{
\ifthenelse{\equal{\@OPTworktype}{bach-thesis}}{
	\gdef\@worktype{Bakalaura darbs}}{
\ifthenelse{\equal{\@OPTworktype}{master-thesis-report}}{
	\gdef\@worktype{Maģistra darba atskaite}}{
\ifthenelse{\equal{\@OPTworktype}{master-thesis}}{
	\gdef\@worktype{Maģistra darbs}}{
	\gdef\@worktype{(darba tips)}}}}}}}

\gdef\@subject{(priekšmeta vārds)}
\gdef\@title{(darba nosaukums)}

\gdef\@author{(autors)}
\gdef\@group{(grupa)}
\gdef\@idnumber{(apl. nr.)}

% Functions to set the global variables:
\newcommand{\university}[1]{\gdef\@university{#1}}
\newcommand{\faculty}[1]{\gdef\@faculty{#1}}
\newcommand{\institute}[1]{\gdef\@institute{#1}}
\newcommand{\department}[1]{\gdef\@department{#1}}

\newcommand{\worktype}[1]{\gdef\@worktype{#1}}
\newcommand{\subject}[1]{\gdef\@subject{#1}}
\renewcommand{\title}[1]{\gdef\@title{#1}}

\renewcommand{\author}[1]{\gdef\@author{#1}}
\newcommand{\group}[1]{\gdef\@group{#1}}
\newcommand{\idnumber}[1]{\gdef\@idnumber{#1}}

% Title page creation:
\newcommand{\maketitle}
{
	\newpage
	\thispagestyle{empty}
	{
		\fontspec{"Tahoma"}
		\setlength{\baselineskip}{0pt}
		\setlength{\parskip}{0pt}
		\setlength{\parindent}{0pt}
		\begin{center}
		{
			\vspace*{1cm}
			\includegraphics[scale=0.5]{./RTU-CoA}\par \vspace{0.5cm}
			\fontsizeZ{22}{0cm}\@university\par	\vspace{0.8cm}
			\fontsizeZ{16.5}{0cm}\@faculty\par	\vspace{0.4cm}
			\fontsizeZ{14}{0cm}\@institute\par	\vspace{0.25cm}
			\fontsizeZ{13}{0cm}\@department\par	\vspace{2.4cm}
			\ifthenelse{\equal{\@OPTthesis}{}}
				{\fontsizeZ{12.3}{11pt}\@worktype\nobreakspace mācību priekšmetā \\ \@subject}
				{\fontsizeZ{12.3}{11pt}\@worktype \\\nobreakspace}
			\par\vspace{1.5cm}
			{
				\fontsizeZ{20}{0pt}
				\setlength{\baselineskip}{1cm}
				\@title\par
			}
		}	\end{center}
		\vspace{5.5cm}
		\fontsizeZ{12}{10}
		\hspace{12cm}\@author\par
		\hspace{12cm}\@group\par
		\hspace{12cm}\@idnumber\par
		\vspace{1cm}
		\begin{center} \monthname\space\number\year \end{center}
	}
	\setcounter{page}{1}
	\setcounter{footnote}{0}
	\newpage
}

% Defining page "matters":
\newcommand{\frontmatter}{\newpage\pagenumbering{roman}}
\newcommand{\mainmatter}{\newpage\pagenumbering{arabic}}

% Defining parts and sections:
\setcounter{secnumdepth}{3}

% Part creation: FIXME: причесать
\newcommand{\fsap}{false}
\newcounter{part}
\renewcommand{\thepart}{\Roman{part}}
\newcommand\part{
	\newpage
	\renewcommand{\fsap}{true}
	\ifthenelse{\equal{\@OPTpagenumperpart}{true}}
	{
		\renewcommand{\thepage}{\thepart~--~\arabic{page}}
		\setcounter{page}{1}
		\setcounter{footnote}{0}
	}{}
   \if@noskipsec \leavevmode \fi
   \par
   \addvspace{4ex}%
   \@afterindentfalse
   \secdef\@part\@spart}

\def\@part[#1]#2{%
    \ifnum \c@secnumdepth >\m@ne
      \refstepcounter{part}%
      \addcontentsline{toc}{part}{\thepart\hspace{1em}#1}%
    \else
      \addcontentsline{toc}{part}{#1}%
    \fi
    {\parindent \z@ \raggedright
     \interlinepenalty \@M
     \normalfont
     \ifnum \c@secnumdepth >\m@ne
       \Large\bfseries \thepart\nobreakspace\MakeLowercase\partname
       \par\nobreak
     \fi
     \huge \bfseries #2%
     \markboth{}{}\par}%
     \noindent\rule{0.99\textwidth}{1pt}
    \nobreak
    \vskip 3ex
    \@afterheading}
\def\@spart#1{%
    {\parindent \z@ \raggedright
     \interlinepenalty \@M
     \normalfont
     \huge \bfseries QQ#1\par    }%

     \nobreak
     \vskip 3ex
     \@afterheading}

% \@startsection {NAME}{LEVEL}{INDENT}{BEFORESKIP}{AFTERSKIP}{STYLE} 
%            optional * [ALTHEADING]{HEADING}
%    Generic command to start a section.  
%    NAME       : e.g., 'subsection'
%    LEVEL      : a number, denoting depth of section -- e.g., chapter=1,
%                 section = 2, etc.  A section number will be printed if
%                 and only if LEVEL < or = the value of the secnumdepth
%                 counter.
%    INDENT     : Indentation of heading from left margin
%    BEFORESKIP : Absolute value = skip to leave above the heading.  
%                 If negative, then paragraph indent of text following 
%                 heading is suppressed.
%    AFTERSKIP  : if positive, then skip to leave below heading,
%                       else - skip to leave to right of run-in heading.
%    STYLE      : commands to set style
%  If '*' missing, then increments the counter.  If it is present, then
%  there should be no [ALTHEADING] argument.  A sectioning command
%  is normally defined to \@startsection + its first six arguments.

% Section creation:\gdef\@first_section_after_part{true}
\newcounter{section}
\renewcommand{\thesection}{\arabic{section}.}
\newcommand\section{\ifthenelse{\equal{\fsap}{false}}{\newpage}{}
	 \renewcommand{\fsap}{false}
	 \@startsection
	{section}
	{1}
	{0pt}
	{-0.4cm}
	{0.16cm}
	{\fontspec{"Arial"}\bold\sizedZ{14}}}

% Subsection creation:
\newcounter{subsection}[section]
\renewcommand{\thesubsection}{\thesection\arabic{subsection}.}
\newcommand\subsection{\@startsection
	{subsection}
	{2}
	{0pt}
	{-0.5cm}
	{0.16cm}
	{\fontspec{"Arial"}\bolditalic\sizedZ{13}}}

% Second subsection creation:
\newcounter{subsubsection}[subsection]
\renewcommand{\thesubsubsection}{\thesubsection\arabic{subsubsection}.}
\newcommand\subsubsection{\@startsection
	{subsubsection}
	{3}
	{0pt}
	{-0.5cm}
	{0.16cm}
	{\fontspec{"Arial"}\bolditalic\sizedZ{12}}}

% Appendix section numbering:
\newcommand{\appendix}
{
	\par
	\setcounter{section}{0}
	\renewcommand{\thesection}{\Alph{section}.}
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Equations:
\numberwithin{equation}{section}
\renewcommand{\theequation}{\mbox{\arabic{section}-\arabic{equation}}}

% 1 - equation label (it will be set to eqn:label),
% 2 - equation
\newcommand{\numeqn}[2]{\begin{equation}\label{eqn:#1}#2\end{equation}}
\newcommand{\numeqnarr}[2]{\begin{equation}\label{eqn:#1}\begin{split}#2\end{split}\end{equation}}

\newcommand{\refeqn}[1]{(\ref{eqn:#1})}
\newcommand{\seeeqn}[1]{(sk. formulu \refeqn{#1})}

% Figures:
\newcounter{figure}[section]
\renewcommand{\thefigure}{\thesection\arabic{figure}.}
\def\fps@figure{hbtp}
\def\ftype@figure{1}
\def\ext@figure{lof}
\def\fnum@figure{\thefigure\nobreakspace\MakeLowercase\figurename}
\newenvironment{figure}
               {\@float{figure}}
               {\end@float}
\newcommand\listoffigures
{
	\section{\listfigurename}
	\@mkboth{\MakeUppercase\listfigurename}
	        {\MakeUppercase\listfigurename}
	\@starttoc{lof}
}
\newcommand*\l@figure{\@dottedtocline{1}{1.5em}{2.3em}}

\long\def\@makecaption#1#2{
    \centering \emph{#1: #2}\par
}

% 1 - figure caption,
% 2 - figure label (it will be set to fig:label),
% 3 - figure width (relative to text width),
% 4 - image filename.
\newcommand{\numfig}[4]
{
	\begin{figure}
		\centering
		\includegraphics[width=#3\textwidth]{#4}
		\caption{#1}\label{fig:#2}
	\end{figure}
}

\newcommand{\reffig}[1]{\ref{fig:#1}}
\newcommand{\seefig}[1]{(sk. \reffig{#1} attēlu)}

% Tables:
\newcounter{table}[section]
\renewcommand{\thetable}{\thesection\arabic{table}.}
\def\fps@table{hbtp}
\def\ftype@table{2}
\def\ext@table{lot}
\def\fnum@table{\thetable\nobreakspace\MakeLowercase\tablename}
\newenvironment{table}
               {\@float{table}}
               {\end@float}
\newenvironment{table*}
               {\@dblfloat{table}}
               {\end@dblfloat}

% 1 - heuristic number to adjust the page breaks,
% 2 - table caption,
% 3 - table label (it will be set to tab:label),
% 4 - number of columns,
% 5 - definition of columns (width,...),
% 6 - header contents.
\newenvironment{newtable}[6]
{
	\xentrystretch{#1}
	
	\ifthenelse{\equal{#2}{}}
	{
		\ifthenelse{\equal{#6}{}}
		{
			\tablefirsthead{\hline}
			\tablehead{
				\multicolumn{#4}{l}{Tabulas turpinājums} \\}
			\tablelasthead{
				\multicolumn{#4}{l}{Tabulas nobeigums} \\}
			\tabletail{\hline \multicolumn{#4}{r}{Tabula turpinās nākamajā lappusē} \\}
		}
		{
			\tablefirsthead{\hline #6 \\ \hline\hline}
			\tablehead{
				\multicolumn{#4}{l}{Tabulas turpinājums} \\ \hline
				#6 \\ \hline}
			\tablelasthead{
				\multicolumn{#4}{l}{Tabulas nobeigums} \\ \hline
				#6 \\ \hline}
			\tabletail{\hline \multicolumn{#4}{r}{Tabula turpinās nākamajā lappusē} \\}
		}
	}
	{
		\tablecaption{#2}\label{tab:#3}
% FIXME: this thing does not work, if header contains \multicolumn
		\ifthenelse{\equal{#6}{}}
		{
			\tablefirsthead{\hline}
			\tablehead{
				\multicolumn{#4}{l}{\thetable~tabulas turpinājums} \\}
			\tablelasthead{
				\multicolumn{#4}{l}{\thetable~tabulas nobeigums} \\}
			\tabletail{\hline \multicolumn{#4}{r}{Tabula turpinās nākamajā lappusē} \\}
		}
		{
			\tablefirsthead{\hline #6 \\ \hline\hline}
			\tablehead{
				\multicolumn{#4}{l}{\thetable~tabulas turpinājums} \\ \hline
				#6 \\ \hline}
			\tablelasthead{
				\multicolumn{#4}{l}{\thetable~tabulas nobeigums} \\ \hline
				#6 \\ \hline}
			\tabletail{\hline \multicolumn{#4}{r}{Tabula turpinās nākamajā lappusē} \\}
		}
	}
	\tablelasttail{}
	
	\begin{center}
	\begin{xtabular}{#5}
}
{
	\end{xtabular}
	\end{center}
}

\newcommand{\nextline}{\\ \hline}
\newcommand{\reftable}[1]{\ref{tab:#1}}
\newcommand{\seetable}[1]{(sk. \reftable{#1} tabulu)}

% Algorithms:
\newcounter{algorithm}[section]
\renewcommand{\thealgorithm}{\thesection\arabic{algorithm}.}
\def\fps@algorithm{hbtp}
\def\ftype@algorithm{1}
\def\ext@algorithm{loa}
\def\fnum@algorithm{\thealgorithm\nobreakspace\MakeLowercase\algorithmname}
\newenvironment{algorithm}
               {\@float{algorithm}}
               {\end@float}
\newcommand{\listofalgorithms}
{
	\section{\listalgorithmname}
	\@mkboth{\MakeUppercase\listalgorithmname}
	        {\MakeUppercase\listalgorithmname}
	\@starttoc{loa}
}
\newcommand*\l@algorithm{\@dottedtocline{1}{1.5em}{2.3em}}

% Custom commands used in algorithms:
\newcommand{\Assign}[2]{\State #1 $\gets$ #2}
\newcommand{\NotEqual}[2]{#1 $\not=$ #2}

% 1 - algorithm caption,
% 2 - algorithm label (it will be set to alg:label),
% 3 - algorithm in 'algpseudocode` format.
\newcommand{\numalg}[3]
{
	\stepcounter{algorithm}
	\begin{algorithm}
		\renewcommand{\baselinestretch}{1.0} % restore the line-spacing
		% put the caption:
		\noindent\emph{\fnum@algorithm: #1}\label{alg:#2}\par
		% put the horizontal line before the algorithm:
		\rule[5pt]{\textwidth}{1pt}\vspace{-10pt}
		\texttt % use a monospace font for an algorithm
		{
			\begin{algorithmic}[1]
				#3
			\end{algorithmic}
		}
		% put the horizontal line after the algorithm:
		\rule[10pt]{\textwidth}{1pt}\vspace{-10pt}
	\end{algorithm}
}

\newcommand{\refalg}[1]{\ref{alg:#1}}
\newcommand{\seealg}[1]{(sk. \refalg{#1} algoritmu)}

% Verbatim environments:
\DefineVerbatimEnvironment
	{SQL}
	{Verbatim}
	{
		baselinestretch=1,
		numbers=left,
		numbersep=2mm,
		fontsize=\relsize{-1},
		tabsize=4
	}

\DefineVerbatimEnvironment
	{shell}
	{Verbatim}
	{
		baselinestretch=1,
		numbers=left,
		numbersep=2mm,
		fontsize=\relsize{-1},
		tabsize=4
	}

\DefineVerbatimEnvironment
	{code}
	{Verbatim}
	{
		baselinestretch=1,
		numbers=left,
		numbersep=2mm,
		fontsize=\relsize{-1},
		tabsize=4
	}

% List environments:
\newenvironment{dotlist}
	{\smallparskip\begin{itemize}\smallparskip}
	{\end{itemize}}

\newenvironment{numlist}
	{\smallparskip\begin{enumerate}\smallparskip}
	{\end{enumerate}}

\newenvironment{desclist}
	{\smallparskip\begin{description}\smallparskip}
	{\end{description}}

% Misc functions: 
\newcommand{\engl}[1]{(angliski: \emph{#1})}

% Section names:
\newcommand\contentsname{Saturs}
\newcommand\listfigurename{Attēlu saraksts}
\newcommand\listtablename{Tabulu saraksts}
\newcommand\listalgorithmname{Algoritmu saraksts}
\newcommand\bibname{Bibliogrāfiskais saraksts}
\newcommand\indexname{Index}
\newcommand\figurename{Attēls}
\newcommand\algorithmname{Algoritms}
\newcommand\tablename{Tabula}
\newcommand\appendixname{Pielikums}
\newcommand\abstractname{Anotācija}
\newcommand\partname{Daļa}

% Calendar:
\newcommand{\monthname}
{
	\ifcase\month
	\or Janvāris
	\or Februāris
	\or Marts
	\or Aprīlis
	\or Maijs
	\or Jūnijs
	\or Jūlijs
	\or Augusts
	\or Septembris
	\or Oktobris
	\or Novembris
	\or Decembris\fi
}

\newcommand{\today}{\number\day.\space\monthname}
\newcommand{\todaywithyear}{\number\year. gada\space\today}

\normalsize
\normalparskip
%\endinput


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




\setcounter{topnumber}{2}
\renewcommand\topfraction{.7}
\setcounter{bottomnumber}{1}
\renewcommand\bottomfraction{.3}
\setcounter{totalnumber}{3}
\renewcommand\textfraction{.2}
\renewcommand\floatpagefraction{.5}
\setcounter{dbltopnumber}{2}
\renewcommand\dbltopfraction{.7}
\renewcommand\dblfloatpagefraction{.5}

  \def\ps@headings{%
    \let\@oddfoot\@empty
    \def\@oddhead{{\slshape\rightmark}\hfil\thepage}%
    \let\@mkboth\markboth
    \def\sectionmark##1{%
      \markright {\MakeUppercase{%
        \ifnum \c@secnumdepth >\m@ne
          \thesection\quad
        \fi
        ##1}}}}

\def\ps@myheadings{%
    \let\@oddfoot\@empty\let\@evenfoot\@empty
    \def\@evenhead{\thepage\hfil\slshape\leftmark}%
    \def\@oddhead{{\slshape\rightmark}\hfil\thepage}%
    \let\@mkboth\@gobbletwo
    \let\sectionmark\@gobble
    \let\subsectionmark\@gobble
    }

%\setcounter{secnumdepth}{3}

%\newcounter {section}
%\newcounter {subsection}[section]
%\newcounter {subsubsection}[subsection]
%\newcounter {paragraph}[subsubsection]
%\newcounter {subparagraph}[paragraph]

%ITEMIZE:
\setlength\leftmargini  {2em}
\setlength\leftmarginii  {2em}
\setlength\leftmarginiii {1.7em}
\setlength\leftmarginiv  {1em}
\setlength\leftmarginv  {1em}
\setlength\leftmarginvi {1em}
\setlength{\labelsep}{1em}





%\setlength  \labelsep  {2em}
%\setlength  \labelwidth{10em}
%\addtolength\labelwidth{0pt}
\@beginparpenalty -\@lowpenalty
\@endparpenalty   -\@lowpenalty
\@itempenalty     -\@lowpenalty
\renewcommand\theenumi{\@arabic\c@enumi}
\renewcommand\theenumii{\@alph\c@enumii}
\renewcommand\theenumiii{\@roman\c@enumiii}
\renewcommand\theenumiv{\@Alph\c@enumiv}
\newcommand\labelenumi{\theenumi.}
\newcommand\labelenumii{(\theenumii)}
\newcommand\labelenumiii{\theenumiii.}
\newcommand\labelenumiv{\theenumiv.}
\renewcommand\p@enumii{\theenumi}
\renewcommand\p@enumiii{\theenumi(\theenumii)}
\renewcommand\p@enumiv{\p@enumiii\theenumiii}
\newcommand\labelitemi{\textbullet}
\newcommand\labelitemii{\normalfont\bfseries \textendash}
\newcommand\labelitemiii{\textasteriskcentered}
\newcommand\labelitemiv{\textperiodcentered}
\newenvironment{description}
               {\list{}{\labelwidth\z@ \itemindent-\leftmargin
                        \let\makelabel\descriptionlabel}}
               {\endlist}
\newcommand*\descriptionlabel[1]{\hspace\labelsep
                                \normalfont\bfseries #1}

  \newenvironment{abstract}{%
      \if@twocolumn
        \section*{\abstractname}%
      \else
        \small
        \begin{center}%
          {\bfseries \abstractname\vspace{-.5em}\vspace{\z@}}%
        \end{center}%
        \quotation
      \fi}
      {\if@twocolumn\else\endquotation\fi}

\newenvironment{verse}
               {\let\\\@centercr
                \list{}{\itemsep      \z@
                        \itemindent   -1.5em%
                        \listparindent\itemindent
                        \rightmargin  \leftmargin
                        \advance\leftmargin 1.5em}%
                \item\relax}
               {\endlist}
\newenvironment{quotation}
               {\list{}{\listparindent 1.5em%
                        \itemindent    \listparindent
                        \rightmargin   \leftmargin
                        \parsep        \z@ \@plus\p@}%
                \item\relax}
               {\endlist}
\newenvironment{quote}
               {\list{}{\rightmargin\leftmargin}%
                \item\relax}
               {\endlist}


\setlength\arraycolsep{5\p@}
\setlength\tabcolsep{6\p@}
\setlength\arrayrulewidth{.4\p@}
\setlength\doublerulesep{2\p@}
\setlength\tabbingsep{\labelsep}
\skip\@mpfootins = \skip\footins
\setlength\fboxsep{3\p@}
\setlength\fboxrule{.4\p@}
%\renewcommand \theequation {\@arabic\c@equation}
%\newcounter{figure}
%\renewcommand \thefigure {\@arabic\c@figure}
%\def\fps@figure{tbp}
%\def\ftype@figure{1}
%\def\ext@figure{lof}
%\def\fnum@figure{\figurename\nobreakspace\thefigure}
%\newenvironment{figure}
%               {\@float{figure}}
%               {\end@float}
%\newenvironment{figure*}
%               {\@dblfloat{figure}}
%               {\end@dblfloat}
%\newcounter{table}
%\renewcommand\thetable{\@arabic\c@table}
%\def\fps@table{tbp}
%\def\ftype@table{2}
%\def\ext@table{lot}
%\def\fnum@table{\tablename\nobreakspace\thetable}
%\newenvironment{table}
%               {\@float{table}}
%               {\end@float}
%\newenvironment{table*}
%               {\@dblfloat{table}}
%               {\end@dblfloat}
\newlength\abovecaptionskip
\newlength\belowcaptionskip
\setlength\abovecaptionskip{10\p@}
\setlength\belowcaptionskip{0\p@}
%\long\def\@makecaption#1#2{%
%  \vskip\abovecaptionskip
%  \sbox\@tempboxa{#1: #2}%
  %\ifdim \wd\@tempboxa >\hsize
%    #1: #2\par
%  \else
%    \global \@minipagefalse
%    \hb@xt@\hsize{\hfil\box\@tempboxa\hfil}%
%  \fi
%  \vskip\belowcaptionskip}
\DeclareOldFontCommand{\rm}{\normalfont\rmfamily}{\mathrm}
\DeclareOldFontCommand{\sf}{\normalfont\sffamily}{\mathsf}
\DeclareOldFontCommand{\tt}{\normalfont\ttfamily}{\mathtt}
\DeclareOldFontCommand{\bf}{\normalfont\bfseries}{\mathbf}
\DeclareOldFontCommand{\it}{\normalfont\itshape}{\mathit}
\DeclareOldFontCommand{\sl}{\normalfont\slshape}{\@nomath\sl}
\DeclareOldFontCommand{\sc}{\normalfont\scshape}{\@nomath\sc}
\DeclareRobustCommand*\cal{\@fontswitch\relax\mathcal}
\DeclareRobustCommand*\mit{\@fontswitch\relax\mathnormal}
\newcommand\@pnumwidth{1.55em}
\newcommand\@tocrmarg{2.55em}
\newcommand\@dotsep{4.5}
\setcounter{tocdepth}{3}
\newcommand\tableofcontents{%
	{
	\renewcommand{\baselinestretch}{1.0}
	%\setlength{\baselineskip}{0pt}
	\setlength{\parskip}{0pt}
	\setlength{\parindent}{0pt}
    \section*{\contentsname
        \@mkboth{%
           \MakeUppercase\contentsname}{\MakeUppercase\contentsname}}%
    \@starttoc{toc}%
    }}
\newcommand*\l@part[2]{%
  \ifnum \c@tocdepth >-2\relax
    \addpenalty\@secpenalty
    \addvspace{2.25em \@plus\p@}%
    \setlength\@tempdima{3em}%
    \begingroup
      \parindent \z@ \rightskip \@pnumwidth
      \parfillskip -\@pnumwidth
      {\leavevmode
       \large \bfseries #1\hfil \hb@xt@\@pnumwidth{\hss #2}}\par
       \nobreak
       \if@compatibility
         \global\@nobreaktrue
         \everypar{\global\@nobreakfalse\everypar{}}%
      \fi
    \endgroup
  \fi}
\newcommand*\l@section[2]{%
  \ifnum \c@tocdepth >\z@
    \addpenalty\@secpenalty
    \addvspace{1.0em \@plus\p@}%
    \setlength\@tempdima{1.5em}%
    \begingroup
      \parindent \z@ \rightskip \@pnumwidth
      \parfillskip -\@pnumwidth
      \leavevmode \bfseries
      \advance\leftskip\@tempdima
      \hskip -\leftskip
      {#1}\nobreak
      {\mdseries\leaders\hbox{$\m@th
        \mkern \@dotsep mu\hbox{.}\mkern \@dotsep
        mu$}\hfill}
      \nobreak
      \hb@xt@\@pnumwidth{\hss #2}\par
    \endgroup
  \fi}
  
%  \def\@dottedtocline#1#2#3#4#5{%
%  \ifnum #1>\c@tocdepth \else
%    \vskip \z@ \@plus.2\p@
%    {\leftskip #2\relax \rightskip \@tocrmarg \parfillskip -\rightskip
%     \parindent #2\relax\@afterindenttrue
%     \interlinepenalty\@M
%     \leavevmode
%     \@tempdima #3\relax
%     \advance\leftskip \@tempdima \null\nobreak\hskip -\leftskip
%     {#4}\nobreak
%     \leaders\hbox{$\m@th
%        \mkern \@dotsep mu\hbox{\hspace{1mm}.}\mkern \@dotsep
%        mu$}\hfill
%     \nobreak
%     \hb@xt@\@pnumwidth{\hfil\normalfont \normalcolor #5}%
  %   #5
%     \par}%
%  \fi}
  
%\newcommand*\l@section{\@dottedtocline{1}{1.0em}{2.0em}}
\newcommand*\l@subsection{\@dottedtocline{2}{1.5em}{2.3em}}
\newcommand*\l@subsubsection{\@dottedtocline{3}{3.8em}{3.2em}}
\newcommand*\l@paragraph{\@dottedtocline{4}{7.0em}{4.1em}}
\newcommand*\l@subparagraph{\@dottedtocline{5}{10em}{5em}}
\newcommand\listoftables{%
    \section{\listtablename}%
      \@mkboth{%
          \MakeUppercase\listtablename}%
         {\MakeUppercase\listtablename}%
    \@starttoc{lot}%
    }
\let\l@table\l@figure
\newdimen\bibindent
\setlength\bibindent{1.5em}
\newenvironment{thebibliography}[1]
     {\section{\bibname}%
      \@mkboth{\MakeUppercase\bibname}{\MakeUppercase\bibname}%
      \list{\@biblabel{\@arabic\c@enumiv}}%
           {\settowidth\labelwidth{\@biblabel{#1}}%
            \leftmargin\labelwidth
            \advance\leftmargin\labelsep
            \@openbib@code
            \usecounter{enumiv}%
            \let\p@enumiv\@empty
            \renewcommand\theenumiv{\@arabic\c@enumiv}}%
      \sloppy
      \clubpenalty4000
      \@clubpenalty \clubpenalty
      \widowpenalty4000%
      \sfcode`\.\@m}
     {\def\@noitemerr
       {\@latex@warning{Empty `thebibliography' environment}}%
      \endlist}
\newcommand\newblock{\hskip .11em\@plus.33em\@minus.07em}
\let\@openbib@code\@empty
\newenvironment{theindex}
               {\if@twocolumn
                  \@restonecolfalse
                \else
                  \@restonecoltrue
                \fi
                \twocolumn[\section*{\indexname}]%
                \@mkboth{\MakeUppercase\indexname}%
                        {\MakeUppercase\indexname}%
                \thispagestyle{plain}\parindent\z@
                \parskip\z@ \@plus .3\p@\relax
                \columnseprule \z@
                \columnsep 35\p@
                \let\item\@idxitem}
               {\if@restonecol\onecolumn\else\clearpage\fi}
\newcommand\@idxitem{\par\hangindent 40\p@}
\newcommand\subitem{\@idxitem \hspace*{20\p@}}
\newcommand\subsubitem{\@idxitem \hspace*{30\p@}}
\newcommand\indexspace{\par \vskip 10\p@ \@plus5\p@ \@minus3\p@\relax}
\renewcommand\footnoterule{%
  \kern-3\p@
  \hrule\@width.4\columnwidth
  \kern2.6\p@}
\newcommand\@makefntext[1]{%
    \parindent 1em%
    \noindent
    \hb@xt@1.8em{\hss\@makefnmark}#1}
%\newcommand\contentsname{Contents}
%\newcommand\listfigurename{List of Figures}
%\newcommand\listtablename{List of Tables}
%\newcommand\refname{References}
%\newcommand\indexname{Index}
%\newcommand\figurename{Figure}
%\newcommand\tablename{Table}

%\newcommand\appendixname{Appendix}
%\newcommand\abstractname{Abstract}
%\def\today{\ifcase\month\or
%  January\or February\or March\or April\or May\or June\or
%  July\or August\or September\or October\or November\or December\fi
%  \space\number\day, \number\year}
\setlength\columnsep{10\p@}
\setlength\columnseprule{0\p@}
%\pagestyle{plain}
%\pagenumbering{arabic}

  \onecolumn

\endinput
%%
%% End of file `article.cls'.

