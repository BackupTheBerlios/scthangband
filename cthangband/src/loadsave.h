#define LOADSAVE_H
#define SFM_SPECIAL	0x80

/*
 * Save the base and ceiling for each skill.
 */
#define SF_SKILL_BASE	0x0002

/*
 * Save the last turn on which remove curse was cast (see curse.diff).
 */
#define SF_CURSE	0x0004

/*
 * Change object_type.info from a byte to a u16b (see pseudoid.diff).
 */
#define SF_16_IDENT	0x0008

/*
 * Save the flag in the death_event array which indicates which events
 * have been observed (see deatheventtext.diff).
 */
#define SF_DEATHEVENTTEXT	0x0010

/*
 * Create an option to suppress the prompt given by autosave (see quietsave.diff).
 */
#define SF_Q_SAVE	0x0020

/*
 * Save the number of monsters observed to have been killed in each quest
 * (see questsee.diff).
 */
#define SF_QUEST_UNKNOWN	0x0040

/*
 * Save more verbose window flags (see windowpri.diff).
 */
#define SF_3D_WINPRI	0x0080

/*
 * Change cave_type.info from a byte to a u16b (see showtrap.diff)
 */
#define SF_16_CAVE_FLAG	0x0100

/*
 * Save MAX_SKILLS in the save file to allow painless addition (but not
 * replacement) of skills.
 */
#define SF_SAVE_MAX_SKILLS 0x0200

/*
 * Reorganise k_info.txt a bit
 */
#define SF_K_INFO_1 0x0400

/*
 * Track which quests have been encountered.
 */
#define SF_QUEST_KNOWN	0x0800

/*
 * Rearrange r_info.txt so that all of the unusual monsters are at the beginning.
 * Add Bokrug and various causes of death to it.
 */
#define SF_R_INFO_1	0x1000

/*
 * Read quests directly into q_info rather than from the dungeon definition.
 */
#define SF_QUEST_DIRECT	0x4000

/*
 * Distribute most ego items based on contents of e_info.txt.
 * Rearranges k_info to make this easier and removes object_type.xtra*.
 */
#define SF_EGO_DISTRO	0x8000
