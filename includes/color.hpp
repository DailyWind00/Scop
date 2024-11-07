#pragma once

#ifdef COLOR_HEADER_C
// Reset
# define Color_Off          "\033[0m"             // Text Reset
# define ResetColor         "\033[0m"             // Text Reset

// Regular Colors
# define Black              "\033[0;30m"          // Black
# define Red                "\033[0;31m"          // Red
# define Green              "\033[0;32m"          // Green
# define Yellow             "\033[0;33m"          // Yellow
# define Blue               "\033[0;34m"          // Blue
# define Purple             "\033[0;35m"          // Purple
# define Cyan               "\033[0;36m"          // Cyan
# define White              "\033[0;37m"          // White
# define Gray               "\033[0;90m"          // Gray
# define Orange             "\033[0;38;5;208m"    // Orange
# define Pink               "\033[0;38;5;206m"    // Pink
# define LightBlue          "\033[0;38;5;117m"    // Light Blue
# define LightGreen         "\033[0;38;5;120m"    // Light Green
# define LightYellow        "\033[0;38;5;228m"    // Light Yellow
# define LightPurple        "\033[0;38;5;141m"    // Light Purple
# define LightCyan          "\033[0;38;5;123m"    // Light Cyan
# define LightGray          "\033[0;38;5;250m"    // Light Gray

// Bold
# define BBlack             "\033[1;30m"          // Bold Black
# define BRed               "\033[1;31m"          // Bold Red
# define BGreen             "\033[1;32m"          // Bold Green
# define BYellow            "\033[1;33m"          // Bold Yellow
# define BBlue              "\033[1;34m"          // Bold Blue
# define BPurple            "\033[1;35m"          // Bold Purple
# define BCyan              "\033[1;36m"          // Bold Cyan
# define BWhite             "\033[1;37m"          // Bold White
# define BGray              "\033[1;90m"          // Bold Gray
# define BOrange            "\033[1;38;5;208m"    // Bold Orange
# define BPink              "\033[1;38;5;206m"    // Bold Pink
# define BLightBlue         "\033[1;38;5;117m"    // Bold Light Blue
# define BLightGreen        "\033[1;38;5;120m"    // Bold Light Green
# define BLightYellow       "\033[1;38;5;228m"    // Bold Light Yellow
# define BLightPurple       "\033[1;38;5;141m"    // Bold Light Purple
# define BLightCyan         "\033[1;38;5;123m"    // Bold Light Cyan
# define BLightGray         "\033[1;38;5;250m"    // Bold Light Gray

// Underline
# define UBlack             "\033[4;30m"          // Underline Black
# define URed               "\033[4;31m"          // Underline Red
# define UGreen             "\033[4;32m"          // Underline Green
# define UYellow            "\033[4;33m"          // Underline Yellow
# define UBlue              "\033[4;34m"          // Underline Blue
# define UPurple            "\033[4;35m"          // Underline Purple
# define UCyan              "\033[4;36m"          // Underline Cyan
# define UWhite             "\033[4;37m"          // Underline White
# define UGray              "\033[4;90m"          // Underline Gray
# define UOrange            "\033[4;38;5;208m"    // Underline Orange
# define UPink              "\033[4;38;5;206m"    // Underline Pink
# define ULightBlue         "\033[4;38;5;117m"    // Underline Light Blue
# define ULightGreen        "\033[4;38;5;120m"    // Underline Light Green
# define ULightYellow       "\033[4;38;5;228m"    // Underline Light Yellow
# define ULightPurple       "\033[4;38;5;141m"    // Underline Light Purple
# define ULightCyan         "\033[4;38;5;123m"    // Underline Light Cyan
# define ULightGray         "\033[4;38;5;250m"    // Underline Light Gray

// Background
# define On_Black           "\033[40m"            // Background Black
# define On_Red             "\033[41m"            // Background Red
# define On_Green           "\033[42m"            // Background Green
# define On_Yellow          "\033[43m"            // Background Yellow
# define On_Blue            "\033[44m"            // Background Blue
# define On_Purple          "\033[45m"            // Background Purple
# define On_Cyan            "\033[46m"            // Background Cyan
# define On_White           "\033[47m"            // Background White
# define On_Gray            "\033[100m"           // Background Gray
# define On_Orange          "\033[48;5;208m"      // Background Orange
# define On_Pink            "\033[48;5;206m"      // Background Pink
# define On_LightBlue       "\033[48;5;117m"      // Background Light Blue
# define On_LightGreen      "\033[48;5;120m"      // Background Light Green
# define On_LightYellow     "\033[48;5;228m"      // Background Light Yellow
# define On_LightPurple     "\033[48;5;141m"      // Background Light Purple
# define On_LightCyan       "\033[48;5;123m"      // Background Light Cyan
# define On_LightGray       "\033[48;5;250m"      // Background Light Gray

// High Intensity
# define IBlack             "\033[0;90m"          // High Intensity Black
# define IRed               "\033[0;91m"          // High Intensity Red
# define IGreen             "\033[0;92m"          // High Intensity Green
# define IYellow            "\033[0;93m"          // High Intensity Yellow
# define IBlue              "\033[0;94m"          // High Intensity Blue
# define IPurple            "\033[0;95m"          // High Intensity Purple
# define ICyan              "\033[0;96m"          // High Intensity Cyan
# define IWhite             "\033[0;97m"          // High Intensity White
# define IGray              "\033[0;90m"          // High Intensity Gray
# define IOrange            "\033[0;38;5;208m"    // High Intensity Orange
# define IPink              "\033[0;38;5;206m"    // High Intensity Pink
# define ILightBlue         "\033[0;38;5;117m"    // High Intensity Light Blue
# define ILightGreen        "\033[0;38;5;120m"    // High Intensity Light Green
# define ILightYellow       "\033[0;38;5;228m"    // High Intensity Light Yellow
# define ILightPurple       "\033[0;38;5;141m"    // High Intensity Light Purple
# define ILightCyan         "\033[0;38;5;123m"    // High Intensity Light Cyan
# define ILightGray         "\033[0;38;5;250m"    // High Intensity Light Gray

// Bold High Intensity
# define BIBlack            "\033[1;90m"          // Bold High Intensity Black
# define BIRed              "\033[1;91m"          // Bold High Intensity Red
# define BIGreen            "\033[1;92m"          // Bold High Intensity Green
# define BIYellow           "\033[1;93m"          // Bold High Intensity Yellow
# define BIBlue             "\033[1;94m"          // Bold High Intensity Blue
# define BIPurple           "\033[1;95m"          // Bold High Intensity Purple
# define BICyan             "\033[1;96m"          // Bold High Intensity Cyan
# define BIWhite            "\033[1;97m"          // Bold High Intensity White
# define BIGray             "\033[1;90m"          // Bold High Intensity Gray
# define BIOrange           "\033[1;38;5;208m"    // Bold High Intensity Orange
# define BIPink             "\033[1;38;5;206m"    // Bold High Intensity Pink
# define BILightBlue        "\033[1;38;5;117m"    // Bold High Intensity Light Blue
# define BILightGreen       "\033[1;38;5;120m"    // Bold High Intensity Light Green
# define BILightYellow      "\033[1;38;5;228m"    // Bold High Intensity Light Yellow
# define BILightPurple      "\033[1;38;5;141m"    // Bold High Intensity Light Purple
# define BILightCyan        "\033[1;38;5;123m"    // Bold High Intensity Light Cyan
# define BILightGray        "\033[1;38;5;250m"    // Bold High Intensity Light Gray

// Backgrounds
# define On_IBlack          "\033[0;100m"         // Backgrounds Black
# define On_IRed            "\033[0;101m"         // Backgrounds Red
# define On_IGreen          "\033[0;102m"         // Backgrounds Green
# define On_IYellow         "\033[0;103m"         // Backgrounds Yellow
# define On_IBlue           "\033[0;104m"         // Backgrounds Blue
# define On_IPurple         "\033[0;105m"         // Backgrounds Purple
# define On_ICyan           "\033[0;106m"         // Backgrounds Cyan
# define On_IWhite          "\033[0;107m"         // Backgrounds White
# define On_IGray           "\033[0;100m"         // Backgrounds Gray
# define On_IOrange         "\033[0;48;5;208m"    // Backgrounds Orange
# define On_IPink           "\033[0;48;5;206m"    // Backgrounds Pink
# define On_ILightBlue      "\033[0;48;5;117m"    // Backgrounds Light Blue
# define On_ILightGreen     "\033[0;48;5;120m"    // Backgrounds Light Green
# define On_ILightYellow    "\033[0;48;5;228m"    // Backgrounds Light Yellow
# define On_ILightPurple    "\033[0;48;5;141m"    // Backgrounds Light Purple
# define On_ILightCyan      "\033[0;48;5;123m"    // Backgrounds Light Cyan
# define On_ILightGray      "\033[0;48;5;250m"    // Backgrounds Light Gray

#endif

#ifdef COLOR_HEADER_CXX
# include <string>
// Reset
# define Color_Off          (string)"\033[0m"             // Text Reset
# define ResetColor         (string)"\033[0m"             // Text Reset

// Regular Colors
# define Black              (string)"\033[0;30m"          // Black
# define Red                (string)"\033[0;31m"          // Red
# define Green              (string)"\033[0;32m"          // Green
# define Yellow             (string)"\033[0;33m"          // Yellow
# define Blue               (string)"\033[0;34m"          // Blue
# define Purple             (string)"\033[0;35m"          // Purple
# define Cyan               (string)"\033[0;36m"          // Cyan
# define White              (string)"\033[0;37m"          // White
# define Gray               (string)"\033[0;90m"          // Gray
# define Orange             (string)"\033[0;38;5;208m"    // Orange
# define Pink               (string)"\033[0;38;5;206m"    // Pink
# define LightBlue          (string)"\033[0;38;5;117m"    // Light Blue
# define LightGreen         (string)"\033[0;38;5;120m"    // Light Green
# define LightYellow        (string)"\033[0;38;5;228m"    // Light Yellow
# define LightPurple        (string)"\033[0;38;5;141m"    // Light Purple
# define LightCyan          (string)"\033[0;38;5;123m"    // Light Cyan
# define LightGray          (string)"\033[0;38;5;250m"    // Light Gray

// Bold
# define BBlack             (string)"\033[1;30m"          // Bold Black
# define BRed               (string)"\033[1;31m"          // Bold Red
# define BGreen             (string)"\033[1;32m"          // Bold Green
# define BYellow            (string)"\033[1;33m"          // Bold Yellow
# define BBlue              (string)"\033[1;34m"          // Bold Blue
# define BPurple            (string)"\033[1;35m"          // Bold Purple
# define BCyan              (string)"\033[1;36m"          // Bold Cyan
# define BWhite             (string)"\033[1;37m"          // Bold White
# define BGray              (string)"\033[1;90m"          // Bold Gray
# define BOrange            (string)"\033[1;38;5;208m"    // Bold Orange
# define BPink              (string)"\033[1;38;5;206m"    // Bold Pink
# define BLightBlue         (string)"\033[1;38;5;117m"    // Bold Light Blue
# define BLightGreen        (string)"\033[1;38;5;120m"    // Bold Light Green
# define BLightYellow       (string)"\033[1;38;5;228m"    // Bold Light Yellow
# define BLightPurple       (string)"\033[1;38;5;141m"    // Bold Light Purple
# define BLightCyan         (string)"\033[1;38;5;123m"    // Bold Light Cyan
# define BLightGray         (string)"\033[1;38;5;250m"    // Bold Light Gray

// Underline
# define UBlack             (string)"\033[4;30m"          // Underline Black
# define URed               (string)"\033[4;31m"          // Underline Red
# define UGreen             (string)"\033[4;32m"          // Underline Green
# define UYellow            (string)"\033[4;33m"          // Underline Yellow
# define UBlue              (string)"\033[4;34m"          // Underline Blue
# define UPurple            (string)"\033[4;35m"          // Underline Purple
# define UCyan              (string)"\033[4;36m"          // Underline Cyan
# define UWhite             (string)"\033[4;37m"          // Underline White
# define UGray              (string)"\033[4;90m"          // Underline Gray
# define UOrange            (string)"\033[4;38;5;208m"    // Underline Orange
# define UPink              (string)"\033[4;38;5;206m"    // Underline Pink
# define ULightBlue         (string)"\033[4;38;5;117m"    // Underline Light Blue
# define ULightGreen        (string)"\033[4;38;5;120m"    // Underline Light Green
# define ULightYellow       (string)"\033[4;38;5;228m"    // Underline Light Yellow
# define ULightPurple       (string)"\033[4;38;5;141m"    // Underline Light Purple
# define ULightCyan         (string)"\033[4;38;5;123m"    // Underline Light Cyan
# define ULightGray         (string)"\033[4;38;5;250m"    // Underline Light Gray

// Background
# define On_Black           (string)"\033[40m"            // Background Black
# define On_Red             (string)"\033[41m"            // Background Red
# define On_Green           (string)"\033[42m"            // Background Green
# define On_Yellow          (string)"\033[43m"            // Background Yellow
# define On_Blue            (string)"\033[44m"            // Background Blue
# define On_Purple          (string)"\033[45m"            // Background Purple
# define On_Cyan            (string)"\033[46m"            // Background Cyan
# define On_White           (string)"\033[47m"            // Background White
# define On_Gray            (string)"\033[100m"           // Background Gray
# define On_Orange          (string)"\033[48;5;208m"      // Background Orange
# define On_Pink            (string)"\033[48;5;206m"      // Background Pink
# define On_LightBlue       (string)"\033[48;5;117m"      // Background Light Blue
# define On_LightGreen      (string)"\033[48;5;120m"      // Background Light Green
# define On_LightYellow     (string)"\033[48;5;228m"      // Background Light Yellow
# define On_LightPurple     (string)"\033[48;5;141m"      // Background Light Purple
# define On_LightCyan       (string)"\033[48;5;123m"      // Background Light Cyan
# define On_LightGray       (string)"\033[48;5;250m"      // Background Light Gray

// High Intensity
# define IBlack             (string)"\033[0;90m"          // High Intensity Black
# define IRed               (string)"\033[0;91m"          // High Intensity Red
# define IGreen             (string)"\033[0;92m"          // High Intensity Green
# define IYellow            (string)"\033[0;93m"          // High Intensity Yellow
# define IBlue              (string)"\033[0;94m"          // High Intensity Blue
# define IPurple            (string)"\033[0;95m"          // High Intensity Purple
# define ICyan              (string)"\033[0;96m"          // High Intensity Cyan
# define IWhite             (string)"\033[0;97m"          // High Intensity White
# define IGray              (string)"\033[0;90m"          // High Intensity Gray
# define IOrange            (string)"\033[0;38;5;208m"    // High Intensity Orange
# define IPink              (string)"\033[0;38;5;206m"    // High Intensity Pink
# define ILightBlue         (string)"\033[0;38;5;117m"    // High Intensity Light Blue
# define ILightGreen        (string)"\033[0;38;5;120m"    // High Intensity Light Green
# define ILightYellow       (string)"\033[0;38;5;228m"    // High Intensity Light Yellow
# define ILightPurple       (string)"\033[0;38;5;141m"    // High Intensity Light Purple
# define ILightCyan         (string)"\033[0;38;5;123m"    // High Intensity Light Cyan
# define ILightGray         (string)"\033[0;38;5;250m"    // High Intensity Light Gray

// Bold High Intensity
# define BIBlack            (string)"\033[1;90m"          // Bold High Intensity Black
# define BIRed              (string)"\033[1;91m"          // Bold High Intensity Red
# define BIGreen            (string)"\033[1;92m"          // Bold High Intensity Green
# define BIYellow           (string)"\033[1;93m"          // Bold High Intensity Yellow
# define BIBlue             (string)"\033[1;94m"          // Bold High Intensity Blue
# define BIPurple           (string)"\033[1;95m"          // Bold High Intensity Purple
# define BICyan             (string)"\033[1;96m"          // Bold High Intensity Cyan
# define BIWhite            (string)"\033[1;97m"          // Bold High Intensity White
# define BIGray             (string)"\033[1;90m"          // Bold High Intensity Gray
# define BIOrange           (string)"\033[1;38;5;208m"    // Bold High Intensity Orange
# define BIPink             (string)"\033[1;38;5;206m"    // Bold High Intensity Pink
# define BILightBlue        (string)"\033[1;38;5;117m"    // Bold High Intensity Light Blue
# define BILightGreen       (string)"\033[1;38;5;120m"    // Bold High Intensity Light Green
# define BILightYellow      (string)"\033[1;38;5;228m"    // Bold High Intensity Light Yellow
# define BILightPurple      (string)"\033[1;38;5;141m"    // Bold High Intensity Light Purple
# define BILightCyan        (string)"\033[1;38;5;123m"    // Bold High Intensity Light Cyan
# define BILightGray        (string)"\033[1;38;5;250m"    // Bold High Intensity Light Gray

// Backgrounds
# define On_IBlack          (string)"\033[0;100m"         // Backgrounds Black
# define On_IRed            (string)"\033[0;101m"         // Backgrounds Red
# define On_IGreen          (string)"\033[0;102m"         // Backgrounds Green
# define On_IYellow         (string)"\033[0;103m"         // Backgrounds Yellow
# define On_IBlue           (string)"\033[0;104m"         // Backgrounds Blue
# define On_IPurple         (string)"\033[0;105m"         // Backgrounds Purple
# define On_ICyan           (string)"\033[0;106m"         // Backgrounds Cyan
# define On_IWhite          (string)"\033[0;107m"         // Backgrounds White
# define On_IGray           (string)"\033[0;100m"         // Backgrounds Gray
# define On_IOrange         (string)"\033[0;48;5;208m"    // Backgrounds Orange
# define On_IPink           (string)"\033[0;48;5;206m"    // Backgrounds Pink
# define On_ILightBlue      (string)"\033[0;48;5;117m"    // Backgrounds Light Blue
# define On_ILightGreen     (string)"\033[0;48;5;120m"    // Backgrounds Light Green
# define On_ILightYellow    (string)"\033[0;48;5;228m"    // Backgrounds Light Yellow
# define On_ILightPurple    (string)"\033[0;48;5;141m"    // Backgrounds Light Purple
# define On_ILightCyan      (string)"\033[0;48;5;123m"    // Backgrounds Light Cyan
# define On_ILightGray      (string)"\033[0;48;5;250m"    // Backgrounds Light Gray
#endif