/*
 * externs.h
 */

/*
 * Automatically generated function definitions
 */

#ifndef INCLUDED_EXTERNS_H
#define INCLUDED_EXTERNS_H

/* birth.c */

#if (defined(BIRTH_C) || defined(FILES_C))
extern int maxstat(int race, int temp, int stat);
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(GENERATE_C))
extern void create_random_name(int race, char *name);
#endif
#if (defined(BIRTH_C) || defined(DUNGEON_C))
extern void player_birth(void);
#endif

/* cave.c */

#if (defined(CAVE_C) || defined(GENERATE_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C))
extern int distance(int y1, int x1, int y2, int x2);
#endif
#if (defined(CAVE_C) || defined(MELEE2_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA1_C))
extern bool los(int y1, int x1, int y2, int x2);
#endif
#if (defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(MELEE2_C) || defined(SPELLS1_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern bool player_can_see_bold(int y, int x);
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(OBJECT1_C))
extern bool no_lite(void);
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern bool cave_valid_bold(int y, int x);
#endif
#if (defined(CAVE_C) || defined(MAIN_AMI_C))
extern void map_info(int y, int x, byte *ap, char *cp, byte *tap, char *tcp);
#endif
#if (defined(CAVE_C) || defined(CMD1_C) || defined(DUNGEON_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(SQUELCH_C) || defined(XTRA2_C))
extern void move_cursor_relative(int row, int col);
#endif
#if (defined(CAVE_C) || defined(CMD1_C) || defined(SPELLS1_C))
extern void print_rel(char c, byte a, int y, int x);
#endif
#if (defined(CAVE_C) || defined(MAIN_X11_C))
extern void highlight_square(int win, int y, int x);
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C))
extern void note_spot(int y, int x);
#endif
#if (defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(DUNGEON_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern void lite_spot(int y, int x);
#endif
#if (defined(CAVE_C) || defined(XTRA1_C))
extern void prt_map(void);
#endif
#if (defined(CAVE_C) || defined(XTRA1_C))
extern void display_map(int *cy, int *cx, int *my, int *mx);
#endif
#if (defined(CAVE_C) || defined(XTRA1_C))
extern void display_wild_map(uint xmin);
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C))
extern void do_cmd_view_map(void);
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(XTRA1_C))
extern void forget_lite(void);
#endif
#if (defined(CAVE_C) || defined(XTRA1_C))
extern void update_lite(void);
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(XTRA1_C))
extern void forget_view(void);
#endif
#if (defined(CAVE_C) || defined(XTRA1_C))
extern void update_view(void);
#endif
#if (defined(CAVE_C) || defined(XTRA1_C))
extern void update_flow(void);
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(POWERS_C))
extern void map_area(void);
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(POWERS_C))
extern void wiz_lite(void);
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(POWERS_C) || defined(SPELLS2_C))
extern void wiz_dark(void);
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(MELEE2_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern void cave_set_feat(int y, int x, int feat);
#endif
#if (defined(CAVE_C) || defined(CMD1_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C))
extern void mmove2(int *y, int *x, int y1, int x1, int y2, int x2);
#endif
#if (defined(CAVE_C) || defined(MELEE2_C) || defined(XTRA2_C))
extern bool projectable(int y1, int x1, int y2, int x2);
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(MONSTER2_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern bool scatter(int *yp, int *xp, int y, int x, int d, bool (*accept)(int, int));
#endif
#if (defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(DUNGEON_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(XTRA2_C))
extern void health_track(int m_idx);
#endif
#if (defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(SPELLS1_C) || defined(XTRA2_C))
extern void monster_race_track(int r_idx);
#endif
#if (defined(CAVE_C) || defined(CMD5_C))
extern void object_kind_track(int k_idx);
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(CMD3_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SQUELCH_C) || defined(XTRA2_C))
extern void object_track(object_type *o_ptr);
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(OBJECT2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void cave_track(const int y, const int x);
#endif
#if (defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(XTRA2_C))
extern void disturb(int stop_stealth);
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern bool is_quest(int level);
#endif

/* cmd1.c */

#if (defined(CMD1_C) || defined(CMD2_C) || defined(POWERS_C))
extern void py_attack(int y, int x);
#endif
#if (defined(CMD1_C) || defined(DUNGEON_C))
extern void do_cmd_attack(void);
#endif
#if (defined(CMD1_C) || defined(OBJECT1_C))
extern void do_cmd_fire(object_type *o_ptr);
#endif
#if (defined(CMD1_C) || defined(POWERS_C))
extern void do_cmd_throw_hard(int mult);
#endif
#if (defined(CMD1_C) || defined(OBJECT1_C))
extern void do_cmd_throw(object_type *o_ptr);
#endif
#if (defined(CMD1_C) || defined(DUNGEON_C))
extern void do_cmd_racial_power(void);
#endif

/* cmd2.c */

#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_go_up(void);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_go_down(void);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_search(void);
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_toggle_sneak(void);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_open(void);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_close(void);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_tunnel(void);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_disarm(void);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_bash(void);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_alter(void);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_spike(void);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_walk(int pickup);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_run(void);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_stay(int pickup);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void do_cmd_rest(void);
#endif
#if (defined(CMD2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C))
extern void move_to(s16b y, s16b x);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C))
extern void run_step(int dir);
#endif

/* cmd3.c */

#if (defined(CMD3_C) || defined(DUNGEON_C))
extern void do_cmd_inven(bool equip);
#endif



#if (defined(CMD3_C) || defined(OBJECT1_C))
extern void do_cmd_wield(object_type *o_ptr);
#endif
#if (defined(CMD3_C) || defined(OBJECT1_C))
extern void do_cmd_takeoff(object_type *o_ptr);
#endif
#if (defined(CMD3_C) || defined(OBJECT1_C))
extern void do_cmd_drop(object_type *o_ptr);
#endif
#if (defined(CMD3_C) || defined(SPELLS2_C))
extern errr do_cmd_destroy_aux(cptr verb, cptr dative, object_type *q_ptr, object_type *o_ptr);
#endif
#if (defined(CMD3_C) || defined(OBJECT1_C))
extern void do_cmd_destroy(object_type *o_ptr);
#endif
#if (defined(CMD3_C) || defined(OBJECT1_C))
extern void do_cmd_hide_object(object_type *o_ptr);
#endif
#if (defined(CMD3_C) || defined(DUNGEON_C))
extern void do_cmd_unhide_objects(void);
#endif
#if (defined(CMD3_C) || defined(DUNGEON_C))
extern void destroy_pack(void);
#endif
#if (defined(CMD3_C) || defined(OBJECT1_C))
extern void do_cmd_observe(object_type *o_ptr);
#endif
#if (defined(CMD3_C) || defined(OBJECT1_C))
extern void do_cmd_uninscribe(object_type *o_ptr);
#endif
#if (defined(CMD3_C) || defined(OBJECT1_C))
extern void do_cmd_inscribe(object_type *o_ptr);
#endif
#if (defined(CMD3_C) || defined(OBJECT1_C))
extern void do_cmd_refill(object_type *j_ptr);
#endif
#if (defined(CMD3_C) || defined(DUNGEON_C))
extern void do_cmd_target(void);
#endif
#if (defined(CMD3_C) || defined(DUNGEON_C))
extern void do_cmd_look(void);
#endif
#if (defined(CMD3_C) || defined(DUNGEON_C))
extern void do_cmd_locate(void);
#endif
#if (defined(CMD3_C) || defined(DUNGEON_C))
extern void do_cmd_query_symbol(void);
#endif
#if (defined(CMD3_C) || defined(OBJECT1_C))
extern void do_cmd_handle(object_type *o_ptr);
#endif

/* cmd4.c */

#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(MAIN_AMI_C) || defined(WIZARD2_C))
extern void do_cmd_redraw(void);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C))
extern void do_cmd_change_name(void);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C))
extern void do_cmd_message_one(void);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C))
extern void do_cmd_messages(void);
#endif
#if (defined(CMD4_C) || defined(FILES_C))
extern void opt_special_effect(const option_type * const op_ptr);
#endif
#if (defined(BIRTH_C) || defined(CMD4_C))
extern void do_cmd_options_aux(int page, cptr info, cptr file);
#endif
#if (defined(CMD4_C) || defined(CMD5_C) || defined(UTIL_C) || defined(XTRA2_C))
extern void clear_f0(char *buf, uint max, cptr UNUSED fmt, va_list UNUSED *vp);
#endif
#if (defined(CMD4_C) || defined(SQUELCH_C))
extern void dump_version(FILE *fff);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C))
extern void do_cmd_options(void);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C))
extern void do_cmd_pref(void);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C))
extern void do_cmd_note(void);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C))
extern void do_cmd_version(void);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C))
extern void do_cmd_feeling(bool FeelingOnly);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C))
extern void do_cmd_load_screen(void);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(SPELLS1_C))
extern void do_cmd_save_screen(void);
#endif
#if (defined(CMD4_C) || defined(MONSTER1_C) || defined(XTRA1_C))
extern void get_symbol_f2(char *buf, uint max, cptr UNUSED fmt, va_list *vp);
#endif
#if (defined(CMD4_C) || defined(FILES_C))
extern long PURE num_kills(void);
#endif
#if (defined(CMD4_C) || defined(FILES_C))
extern void do_cmd_knowledge_chaos_features(void);
#endif
#if (defined(CMD4_C) || defined(XTRA1_C))
extern bool shops_good(int town);
#endif
#if (defined(CMD4_C) || defined(XTRA1_C))
extern void shops_display(int town);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C))
extern void do_cmd_knowledge(void);
#endif

/* cmd5.c */

#if (defined(BIRTH_C) || defined(CMD5_C) || defined(LOAD_C) || defined(SAVE_C) || defined(XTRA1_C))
extern magic_type *num_to_spell(int i);
#endif
#if (defined(CMD5_C) || defined(XTRA1_C))
extern int spell_skill(const magic_type *s_ptr);
#endif
#if (defined(CMD5_C) || defined(MONSTER1_C) || defined(SPELLS2_C))
extern void evaluate_text_f3(char *buf, uint max, cptr UNUSED fmt,
	va_list *vp)
;
#endif
#if (defined(CMD1_C) || defined(CMD5_C))
extern u16b spell_energy(u16b skill,u16b min);
#endif
#if (defined(CMD5_C) || defined(STORE_C))
extern int get_spirit(int *sn, cptr prompt, bool call);
#endif
#if (defined(CMD5_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(STORE_C))
extern bool PURE item_tester_spells(object_ctype *o_ptr);
#endif
#if (defined(CMD5_C) || defined(OBJECT2_C))
extern void display_spells(int y, int x, object_ctype *o_ptr);
#endif
#if (defined(CMD3_C) || defined(CMD5_C) || defined(OBJECT1_C) || defined(STORE_C))
extern void do_cmd_browse(object_type *o_ptr);
#endif
#if (defined(CMD5_C) || defined(OBJECT1_C))
extern void do_cmd_study(object_type *o_ptr);
#endif
#if (defined(CMD5_C) || defined(OBJECT1_C))
extern void do_cmd_cast(object_type *o_ptr);
#endif
#if (defined(CMD5_C) || defined(OBJECT1_C))
extern void do_cmd_cantrip(object_type *o_ptr);
#endif
#if (defined(CMD5_C) || defined(DUNGEON_C))
extern void do_cmd_invoke(void);
#endif
#if (defined(CMD5_C) || defined(DUNGEON_C))
extern void do_cmd_mindcraft(void);
#endif
#if (defined(CMD5_C) || defined(XTRA1_C))
extern void display_spell_list(void);
#endif
#if (defined(CHECK_ARRAYS)) && (defined(CMD5_C) || defined(INIT2_C))
extern void check_magic_info(void);
#endif

/* cmd6.c */

#if (defined(CMD6_C) || defined(OBJECT1_C))
extern cptr describe_object_power(object_ctype *o_ptr);
#endif
#if (defined(CMD5_C) || defined(CMD6_C))
extern s16b item_use_energy(object_ctype *o_ptr);
#endif
#if (defined(CMD3_C) || defined(CMD6_C) || defined(OBJECT1_C))
extern void do_cmd_eat_food(object_type *o_ptr);
#endif
#if (defined(CMD3_C) || defined(CMD6_C) || defined(OBJECT1_C))
extern void do_cmd_quaff_potion(object_type *o_ptr);
#endif
#if (defined(CMD6_C) || defined(POWERS_C) || defined(XTRA2_C))
extern bool curse_armor(void);
#endif
#if (defined(CMD6_C) || defined(POWERS_C) || defined(XTRA2_C))
extern bool curse_weapon(void);
#endif
#if (defined(CMD3_C) || defined(CMD6_C) || defined(OBJECT1_C))
extern void do_cmd_read_scroll(object_type *o_ptr);
#endif
#if (defined(CMD6_C) || defined(OBJECT1_C))
extern void get_device_chance(object_ctype *o_ptr, int *num, int *denom);
#endif
#if (defined(CMD3_C) || defined(CMD6_C) || defined(OBJECT1_C))
extern void do_cmd_use_staff(object_type *o_ptr);
#endif
#if (defined(CMD3_C) || defined(CMD6_C) || defined(OBJECT1_C))
extern void do_cmd_aim_wand(object_type *o_ptr);
#endif
#if (defined(CMD3_C) || defined(CMD6_C) || defined(OBJECT1_C))
extern void do_cmd_zap_rod(object_type *o_ptr);
#endif
#if (defined(CHECK_ARRAYS)) && (defined(CMD6_C) || defined(INIT2_C))
extern void check_activation_info(void);
#endif
#if (defined(CMD3_C) || defined(CMD6_C) || defined(OBJECT1_C))
extern void do_cmd_activate(object_type *o_ptr);
#endif

/* dungeon.c */

#if (defined(DUNGEON_C) || defined(MELEE2_C))
extern u16b ident_power(object_ctype *o_ptr);
#endif
#if (defined(DUNGEON_C) || defined(SQUELCH_C))
extern bool PURE k_can_curse(int k_idx);
#endif
#if (defined(DUNGEON_C) || defined(OBJECT1_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(SQUELCH_C))
extern int PURE find_feeling(object_ctype *o_ptr);
#endif
#if (defined(DUNGEON_C) || defined(SQUELCH_C))
extern bool k_can_sense(int k_idx);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(WIZARD2_C))
extern void change_level(s16b new_level, byte come_from);
#endif
#if (defined(DUNGEON_C) || defined(POWERS_C))
extern bool psychometry(void);
#endif
#if (defined(CMD3_C) || defined(DUNGEON_C))
extern void curse(object_type *o_ptr);
#endif
#if (defined(DUNGEON_C) || defined(SQUELCH_C) || defined(STORE_C))
extern void process_command(void);
#endif
#if (defined(DUNGEON_C) || defined(POWERS_C))
extern void process_some_user_pref_files(void);
#endif
#if (defined(DUNGEON_C) || defined(MAIN_CRB_C) || defined(MAIN_EMX_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(MAIN_C))
extern void play_game(bool new_game);
#endif

/* files.c */

#if (defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(INIT2_C) || defined(MAIN_C) || defined(WIZARD1_C))
extern void safe_setuid_drop(void);
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(INIT2_C) || defined(WIZARD1_C))
extern void safe_setuid_grab(void);
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(INIT2_C))
extern cptr add_stats(s16b sex, s16b race, s16b template, bool maximise, s16b st, s16b in, s16b wi, s16b dx, s16b co, s16b ch, cptr name);
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(OBJECT1_C))
extern cptr process_pref_file_aux(char *buf, u16b *sf_flags);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(MAIN_AMI_C) || defined(OBJECT1_C))
extern errr process_pref_file(cptr name);
#endif
#if (defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_C))
extern errr check_time(void);
#endif
#if (defined(FILES_C) || defined(MAIN_C))
extern errr check_time_init(void);
#endif
#if (defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_C))
extern errr check_load(void);
#endif
#if (defined(FILES_C) || defined(MAIN_C))
extern errr check_load_init(void);
#endif
#if (defined(FILES_C) || defined(XTRA1_C))
extern void prt_nums(cptr txt, int y, int x, int l, int cur, int max);
#endif
#if (defined(FILES_C) || defined(OBJECT1_C))
extern void weapon_stats(object_type *o_ptr, int slay, s16b *tohit, s16b *todam, s16b *weap_blow, s16b *mut_blow, s32b *damage);
#endif
#if (defined(FILES_C) || defined(INIT2_C) || defined(XTRA1_C))
extern void equippy_f0(char *buf, uint max, cptr UNUSED fmt, va_list UNUSED *vp);
#endif
#if (defined(FILES_C) || defined(OBJECT1_C))
extern int equip_mod(int i);
#endif
#if (defined(DUNGEON_C) || defined(FILES_C))
extern bool PURE player_has_flag_known(int set, u32b flag);
#endif
#if (defined(CMD4_C) || defined(FILES_C))
extern void dump_history(FILE *fff);
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(WIZARD2_C) || defined(XTRA1_C))
extern bool display_player(int mode);
#endif
#if (defined(CMD4_C) || defined(FILES_C))
extern void file_character(cptr name);
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C))
extern void do_cmd_help(cptr name);
#endif
#if (defined(FILES_C) || defined(INIT1_C) || defined(UTIL_C))
extern int color_char_to_attr(char c);
#endif
#if (defined(FILES_C) || defined(XTRA1_C))
extern cptr cur_help_str(void);
#endif
#if (defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(SQUELCH_C) || defined(STORE_C) || defined(UTIL_C))
extern void help_track(cptr str);
#endif
#if (defined(CMD4_C) || defined(FILES_C))
extern void show_file(cptr name, cptr what);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C))
extern void show_link(cptr link);
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(INIT2_C))
extern void init_help_files(void);
#endif
#if (defined(FILES_C) || defined(XTRA1_C))
extern void display_help_page(cptr str);
#endif
#if (defined(BIRTH_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_EMX_C) || defined(MAIN_WIN_C) || defined(MAIN_C))
extern void process_player_name(void);
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C))
extern bool get_name(void);
#endif
#if (defined(DUNGEON_C) || defined(FILES_C))
extern void do_cmd_suicide(void);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_CRB_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C))
extern void do_cmd_save_game(bool is_autosave);
#endif
#if (defined(FILES_C) || defined(MAIN_CRB_C) || defined(MAIN_WIN_C))
extern int highscore_fd;
#endif
#if (defined(FILES_C) || defined(MAIN_CRB_C) || defined(MAIN_WIN_C))
extern void display_scores_aux(int from, int to, int note, high_score *score);
#endif
#if (defined(FILES_C) || defined(MAIN_EMX_C) || defined(MAIN_C))
extern void display_scores(int from, int to);
#endif
#if (defined(FILES_C) || defined(STORE_C))
extern void template_score(int ptemplate);
#endif
#if (defined(FILES_C) || defined(STORE_C))
extern void race_score(int race_num);
#endif
#if (defined(FILES_C) || defined(MAIN_CRB_C) || defined(MAIN_WIN_C))
extern errr predict_score(void);
#endif
#if (defined(DUNGEON_C) || defined(FILES_C))
extern void close_game(void);
#endif
#if (defined(FILES_C) || defined(MAIN_CAP_C) || defined(MAIN_GCU_C))
extern void exit_game_panic(void);
#endif
#if (defined(CMD1_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT1_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(XTRA2_C))
extern void get_rnd_line_f1(char *buf, uint max, cptr UNUSED fmt, va_list *vp);
#endif
#if (defined(HANDLE_SIGNALS)) && (defined(FILES_C) || defined(MAIN_SLA_C))
extern Signal_Handler_t (*signal_aux)(int, Signal_Handler_t);
#endif
#if (defined(FILES_C) || defined(MAIN_EMX_C) || defined(MAIN_GTK_C) || defined(MAIN_ROS_C) || defined(MAIN_C))
extern void signals_init(void);
#endif
#if (defined(ANGBAND_H) || defined(H_DEFINE_H))
extern void assert_fail(cptr error, cptr file, int line);
#endif

/* generate.c */

#if (defined(CMD5_C) || defined(GENERATE_C))
extern void generate_spirit_name(spirit_type *s_ptr);
#endif
#if (defined(BIRTH_C) || defined(GENERATE_C))
extern void generate_spirit_names(void);
#endif
#if (defined(GENERATE_C) || defined(MONSTER2_C) || defined(WIZARD2_C))
extern bool PURE cave_empty_bold_p(int y, int  x);
#endif
#if (defined(DUNGEON_C) || defined(GENERATE_C) || defined(MONSTER2_C))
extern bool PURE daytime_p(void);
#endif
#if (defined(CMD2_C) || defined(GENERATE_C) || defined(SPELLS2_C))
extern void replace_secret_door(int y, int x);
#endif
#if (defined(DUNGEON_C) || defined(GENERATE_C))
extern void generate_cave(void);
#endif

/* init1.c */

#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_r_event(char *buf, header *head, vptr *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_z_info(char *buf, header *head, vptr UNUSED *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_f_info(char *buf, header *head, vptr *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_v_info(char *buf, header *head, vptr *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_k_info(char *buf, header *head, vptr *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_o_base(char *buf, header *head, vptr *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_u_info(char *buf, header *head, vptr *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_a_info(char *buf, header *head, vptr *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_e_info(char *buf, header *head, vptr *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_r_info(char *buf, header *head, vptr *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_dun_defs(char *buf, header *head, vptr *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_town_defs(char *buf, header *head, vptr *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_q_list(char *buf, header *head, vptr *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_s_info(char *buf, header *head, vptr *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr parse_macro_info(char *buf, header *head, vptr *extra);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern errr init_info_txt(FILE *fp, char *buf, header *head);
#endif

/* init2.c */

#if (defined(INIT2_C) || defined(MAIN_CRB_C) || defined(MAIN_EMX_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(MAIN_XXX_C) || defined(MAIN_C))
extern void init_file_paths(cptr path);
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C) || defined(MAIN_XPJ_C))
extern s16b error_idx;
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C))
extern s16b error_line;
#endif
#if (defined(INIT2_C) || defined(MAIN_CRB_C) || defined(MAIN_ROS_C))
extern errr (*check_modification_date_hook)(int fd, cptr template_file);
#endif
#if (defined(INIT2_C) || defined(STORE_C))
extern s16b store_table[MAX_STORE_TYPES][STORE_CHOICES];
#endif
#if (defined(INIT2_C) || defined(MAIN_CRB_C) || defined(MAIN_EMX_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(MAIN_XXX_C) || defined(MAIN_C))
extern void init_angband(void);
#endif
#if (defined(INIT2_C) || defined(MAIN_GTK_C) || defined(MAIN_WIN_C) || defined(MAIN_C))
extern void cleanup_angband(void);
#endif

/* load.c */

#if (defined(DUNGEON_C) || defined(LOAD_C))
extern bool load_player(bool new_game);
#endif

/* loadsave.c */

#if (defined(LOAD_C) || defined(LOADSAVE_C) || defined(SAVE_C))
extern byte sf_major;
#endif
#if (defined(LOAD_C) || defined(LOADSAVE_C) || defined(SAVE_C))
extern byte sf_minor;
#endif
#if (defined(LOAD_C) || defined(LOADSAVE_C) || defined(SAVE_C))
extern byte sf_patch;
#endif
#if (defined(LOAD_C) || defined(LOADSAVE_C) || defined(SAVE_C))
extern u16b sf_flags_sf[MAX_SF_VAR];
#endif
#if (defined(LOAD_C) || defined(LOADSAVE_C) || defined(SAVE_C))
extern u32b sf_xtra;
#endif
#if (defined(LOAD_C) || defined(LOADSAVE_C) || defined(SAVE_C))
extern u32b sf_when;
#endif
#if (defined(LOAD_C) || defined(LOADSAVE_C) || defined(SAVE_C))
extern u16b sf_lives;
#endif
#if (defined(LOAD_C) || defined(LOADSAVE_C) || defined(SAVE_C))
extern u16b sf_saves;
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(LOAD_C) || defined(LOADSAVE_C) || defined(SAVE_C) || defined(SQUELCH_C))
extern const u16b sf_flags_now[MAX_SF_VAR];
#endif
#if (defined(FILES_C) || defined(LOAD_C) || defined(LOADSAVE_C) || defined(SAVE_C) || defined(SQUELCH_C))
extern s16b convert_k_idx(s16b idx, const u16b *from_v, const u16b *to_v);
#endif
#if (defined(FILES_C) || defined(LOAD_C) || defined(LOADSAVE_C) || defined(SAVE_C))
extern s16b convert_r_idx(s16b idx, const u16b *from_v, const u16b *to_v);
#endif
#if (defined(LOAD_C) || defined(LOADSAVE_C) || defined(SAVE_C))
extern s16b convert_owner(s16b idx, const u16b *from_v, const u16b *to_v);
#endif
#if (defined(CMD4_C) || defined(LOADSAVE_C) || defined(OBJECT1_C))
extern void current_flags(u16b *flags);
#endif
#if (defined(LOAD_C) || defined(LOADSAVE_C) || defined(SAVE_C))
extern void current_version(u16b *flags, byte *major, byte *minor, byte *patch);
#endif
#if (defined(LOAD_C) || defined(LOADSAVE_C) || defined(SAVE_C))
extern bool has_flag(int flag);
#endif

/* maid-x11.c */

#if ((defined(USE_X11) || defined(USE_XAW) || defined(USE_XPJ) || defined(USE_GTK))) && (defined(MAID_X11_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C))
extern u32b create_pixel(Display *dpy, byte red, byte green, byte blue);
#endif
#if ((defined(USE_X11) || defined(USE_XAW) || defined(USE_XPJ) || defined(USE_GTK))) && (defined(MAID_X11_C) || defined(MAIN_GTK_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C))
extern cptr get_default_font(int term_num);
#endif
#if (((defined(USE_X11) || defined(USE_XAW) || defined(USE_XPJ) || defined(USE_GTK))) && defined(USE_GRAPHICS)) && (defined(MAID_X11_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C))
extern XImage *ReadBMP(Display *dpy, char *Name);
#endif
#if (((defined(USE_X11) || defined(USE_XAW) || defined(USE_XPJ) || defined(USE_GTK))) && defined(USE_GRAPHICS)) && (defined(MAID_X11_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C))
extern bool smoothRescaling;
#endif
#if (((defined(USE_X11) || defined(USE_XAW) || defined(USE_XPJ) || defined(USE_GTK))) && defined(USE_GRAPHICS)) && (defined(MAID_X11_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C))
extern XImage *ResizeImage(Display *dpy, XImage *Im,
                           int ix, int iy, int ox, int oy)
;
#endif

/* main-ami.c */

#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern int GFXW, GFXH, GFXB;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern char modestr[ 256 ];
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern struct Library *GadToolsBase;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern struct Library *AslBase;
#endif
#if ((defined(USE_AMI)) && defined(__GNUC__)) && (defined(MAIN_AMI_C))
extern struct ReqToolsBase *ReqToolsBase;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern struct Library *DiskfontBase;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern struct IntuitionBase *IntuitionBase;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern struct GfxBase *GfxBase;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern struct Library *IFFBase;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern struct Library *CyberGfxBase;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern struct Library *DataTypesBase;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern struct Device *ConsoleDev;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern struct Device *ConsoleDevice;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern bool use_mask;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern bool use_bkg;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern bool nasty_optimise_gfx;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern bool amiga_palette;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern bool block_nasty_gfx;
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern struct NewMenu post_item[];
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C) || defined(MAIN_ROS_C))
extern struct NewMenu menu_ptr[MENUMAX];
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern struct NewMenu window_menu[];
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C) || defined(MAIN_C))
extern const char help_ami[];
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C) || defined(MAIN_C))
extern errr init_ami(int argc, char **argv);
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void amiga_open_libs( void );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void open_term( int n, bool doall );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void close_term( int n );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern int read_menus( void );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern int read_prefs( void );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern errr amiga_event( int v );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void tomb_str( int y, char *str );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void handle_rawkey( UWORD code, UWORD qual, APTR addr );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void handle_menupick( int mnum, int term );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void load_palette( void );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern ULONG trans( byte g );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern int create_menus( void );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void update_menus( void );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void free_sound( void );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void put_gfx_map( term_data *td, int x, int y, int c, int a );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern struct BitMap *alloc_bitmap( int width, int height, int depth, ULONG flags, struct BitMap *friend );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void free_bitmap( struct BitMap *bitmap );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void scale_bitmap( struct BitMap *srcbm, int srcw, int srch, struct BitMap *dstbm, int dstw, int dsth );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void remap_bitmap( struct BitMap *srcbm, struct BitMap *dstbm, long *pens, int width, int height );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern int depth_of_bitmap( struct BitMap *bm );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void amiga_show( char *str );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void amiga_redefine_colours( void );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void amiga_makepath( char *name );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void amiga_save_palette( void );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void amiga_load_palette( void );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void amiga_hs_to_ascii(void);
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void amiga_user_name( char *buf );
#endif
#if (defined(USE_AMI)) && (defined(MAIN_AMI_C))
extern void amiga_write_user_name( char *name );
#endif

/* main-cap.c */

#if ((defined(USE_CAP)) && defined(USE_HARDCODE)) && (defined(MAIN_CAP_C) || defined(MAIN_C))
extern const char help_cap[];
#endif
#if ((defined(USE_CAP)) && !(defined(USE_HARDCODE))) && (defined(MAIN_CAP_C) || defined(MAIN_C))
extern const char help_cap[];
#endif
#if (defined(USE_CAP)) && (defined(MAIN_CAP_C) || defined(MAIN_C))
extern errr init_cap(int argc, char **argv);
#endif

/* main-crb.c */

#if (((defined(MACINTOSH) || defined(MACH_O_CARBON))) && (defined(MACH_O_CARBON) || defined(MAC_MPW))) && (defined(MAIN_CRB_C) || defined(MAIN_MAC_C))
extern u32b _fcreator;
#endif
#if (((defined(MACINTOSH) || defined(MACH_O_CARBON))) && (defined(MACH_O_CARBON) || defined(MAC_MPW))) && (defined(H_CONFIG_H) || defined(MAIN_CRB_C) || defined(MAIN_MAC_C))
extern u32b _ftype;
#endif
#if (((defined(MACINTOSH) || defined(MACH_O_CARBON))) && defined(MACH_O_CARBON)) && (defined(MAIN_CRB_C))
extern void open_aux_crb(cptr pathname);
#endif
#if (((defined(MACINTOSH) || defined(MACH_O_CARBON))) && defined(MAC_MPW)) && (defined(MAIN_CRB_C))
extern void convert_pathname(char* path);
#endif

/* main-dos.c */

#if (defined(USE_DOS)) && (defined(MAIN_DOS_C) || defined(MAIN_C))
extern const char help_dos[];
#endif
#if (defined(USE_DOS)) && (defined(MAIN_DOS_C) || defined(MAIN_C))
extern errr init_dos(int argc, char **argv);
#endif

/* main-emx.c */

#if (((defined(USE_EMX)) && !(defined(EMXPM))) && !(defined(__EMX__CLIENT__))) && (defined(MAIN_EMX_C) || defined(MAIN_C))
extern const char help_emx[];
#endif
#if (((defined(USE_EMX)) && !(defined(EMXPM))) && !(defined(__EMX__CLIENT__))) && (defined(MAIN_EMX_C) || defined(MAIN_C))
extern errr init_emx(int argc, char **argv);
#endif
#if (((defined(USE_EMX)) && !(defined(EMXPM))) && defined(__EMX__CLIENT__)) && (defined(MAIN_EMX_C))
extern void moveClientWindow(int x, int y);
#endif
#if ((defined(USE_EMX)) && defined(EMXPM)) && (defined(MAIN_EMX_C))
extern void emx_init_term(term *t, void *main_instance, int n);
#endif
#if ((defined(USE_EMX)) && defined(EMXPM)) && (defined(MAIN_EMX_C) || defined(MAIN_C))
extern errr init_emx(int argc, char **argv);
#endif
#if ((defined(USE_EMX)) && defined(EMXPM)) && (defined(MAIN_EMX_C))
extern void angbandThread(void *arg);
#endif

/* main-gcu.c */

#if ((defined(USE_GCU)) && defined(USE_NCURSES)) && (defined(MAIN_GCU_C) || defined(MAIN_C))
extern const char help_gcu[];
#endif
#if ((defined(USE_GCU)) && !(defined(USE_NCURSES))) && (defined(MAIN_GCU_C) || defined(MAIN_C))
extern const char help_gcu[];
#endif
#if (defined(USE_GCU)) && (defined(MAIN_GCU_C) || defined(MAIN_C))
extern errr init_gcu(int argc, char **argv);
#endif

/* main-gtk.c */

#if (defined(USE_GTK)) && (defined(MAIN_GTK_C) || defined(MAIN_C))
extern const char help_gtk[];
#endif
#if (defined(USE_GTK)) && (defined(MAIN_GTK_C) || defined(MAIN_C))
extern errr init_gtk(int argc, char **argv);
#endif

/* main-ibm.c */

#if (defined(USE_IBM)) && (defined(MAIN_IBM_C) || defined(MAIN_C))
extern const char help_ibm[];
#endif
#if (defined(USE_IBM)) && (defined(MAIN_IBM_C) || defined(MAIN_C))
extern errr init_ibm(int argc, char **argv);
#endif

/* main-lsl.c */

#if (defined(USE_LSL)) && (defined(MAIN_LSL_C) || defined(MAIN_C))
extern const char help_lsl[];
#endif
#if (defined(USE_LSL)) && (defined(MAIN_LSL_C) || defined(MAIN_C))
extern errr init_lsl(int argc, char **argv);
#endif

/* main-mac.c */

#if (defined(MACINTOSH)) && (defined(MAIN_CRB_C) || defined(MAIN_MAC_C))
extern Boolean open_when_ready;
#endif
#if (defined(MACINTOSH)) && (defined(MAIN_CRB_C) || defined(MAIN_MAC_C))
extern Boolean quit_when_ready;
#endif
#if ((defined(MACINTOSH)) && defined(USE_SFL_CODE)) && (defined(MAIN_MAC_C))
extern AEEventHandlerUPP AEH_Start_UPP;
#endif
#if ((defined(MACINTOSH)) && defined(USE_SFL_CODE)) && (defined(MAIN_MAC_C))
extern AEEventHandlerUPP AEH_Quit_UPP;
#endif
#if ((defined(MACINTOSH)) && defined(USE_SFL_CODE)) && (defined(MAIN_MAC_C))
extern AEEventHandlerUPP AEH_Print_UPP;
#endif
#if ((defined(MACINTOSH)) && defined(USE_SFL_CODE)) && (defined(MAIN_MAC_C))
extern AEEventHandlerUPP AEH_Open_UPP;
#endif
#if ((defined(MACINTOSH) || defined(MACH_O_CARBON))) && (defined(MAIN_CRB_C) || defined(MAIN_MAC_C))
extern void get_version_mac(int *v);
#endif

/* main-ros.c */

#if (defined(ACORN)) && (defined(FILES_C) || defined(LOAD_C) || defined(MAIN_ROS_C) || defined(SAVE_C) || defined(UTIL_C))
extern FILE *my_fopen(cptr f, cptr m);
#endif
#if (defined(ACORN)) && (defined(BIRTH_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_XPJ_C) || defined(SAVE_C) || defined(SQUELCH_C) || defined(UTIL_C) || defined(WIZARD1_C))
extern errr my_fclose(FILE *fp);
#endif
#if (defined(ACORN)) && (defined(INIT2_C) || defined(MAIN_ROS_C) || defined(SAVE_C) || defined(UTIL_C))
extern int fd_make(cptr file, int mode);
#endif
#if (defined(ACORN)) && (defined(CMD4_C) || defined(FILES_C) || defined(LOAD_C) || defined(MAIN_ROS_C) || defined(SAVE_C) || defined(UTIL_C) || defined(XTRA2_C))
extern errr fd_kill(cptr file);
#endif
#if (defined(ACORN)) && (defined(MAIN_ROS_C) || defined(SAVE_C) || defined(UTIL_C))
extern errr fd_move(cptr old, cptr new);
#endif
#if (defined(ACORN)) && (defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MAIN_CRB_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(UTIL_C))
extern int fd_open(cptr path, int flags);
#endif
#if (defined(ACORN)) && (defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MAIN_CRB_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(SAVE_C) || defined(UTIL_C))
extern errr fd_close(int handle);
#endif
#if (defined(ACORN)) && (defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MAIN_ROS_C) || defined(UTIL_C))
extern errr fd_read(int handle, char *buf, size_t nbytes);
#endif
#if (defined(ACORN)) && (defined(FILES_C) || defined(INIT2_C) || defined(MAIN_ROS_C) || defined(UTIL_C))
extern errr fd_write(int handle, const char *buf, size_t nbytes);
#endif
#if (defined(ACORN)) && (defined(FILES_C) || defined(MAIN_ROS_C) || defined(UTIL_C))
extern errr fd_seek(int handle, long offset);
#endif
#if (defined(ACORN)) && (defined(FILES_C) || defined(MAIN_ROS_C) || defined(UTIL_C))
extern errr fd_lock(int handle, int what);
#endif
#if (defined(ACORN)) && (defined(MAIN_ROS_C) || defined(UTIL_C))
extern errr path_temp(char *buf, int max);
#endif
#if ((defined(ACORN)) && defined(USE_FILECACHE)) && (defined(MAIN_ROS_C))
extern FILE *cached_fopen(cptr name, cptr mode);
#endif
#if ((defined(ACORN)) && defined(USE_FILECACHE)) && (defined(MAIN_ROS_C))
extern errr cached_fclose(FILE *fch_);
#endif
#if ((defined(ACORN)) && defined(USE_FILECACHE)) && (defined(MAIN_ROS_C))
extern errr cached_fgets(FILE *fch_, char *buffer, size_t max_len);
#endif

/* main-sla.c */

#if (defined(USE_SLA)) && (defined(MAIN_SLA_C) || defined(MAIN_C))
extern const char help_sla[];
#endif
#if (defined(USE_SLA)) && (defined(MAIN_SLA_C) || defined(MAIN_C))
extern errr init_sla(int argc, char **argv);
#endif

/* main-vcs.c */

#if (defined(USE_VCS)) && (defined(MAIN_VCS_C) || defined(MAIN_C))
extern const char help_vcs[];
#endif
#if (defined(USE_VCS)) && (defined(MAIN_VCS_C) || defined(MAIN_C))
extern errr init_vcs(int argc, char** argv);
#endif

/* main-vme.c */

#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern char e2a[];
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern char a2e[];
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern char DISP[26];
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_AMI_C) || defined(MAIN_CAP_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_GCU_C) || defined(MAIN_GTK_C) || defined(MAIN_IBM_C) || defined(MAIN_MAC_C) || defined(MAIN_VME_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C))
extern int rows, cols;
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_CAP_C) || defined(MAIN_VME_C))
extern int curx;
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_CAP_C) || defined(MAIN_VME_C))
extern int cury;
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_IBM_C) || defined(MAIN_VME_C))
extern byte VirtualScreen[2048];
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern byte ScreenAttr[2048];
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_IBM_C) || defined(MAIN_VME_C))
extern byte wiper[256];
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_IBM_C) || defined(MAIN_VME_C))
extern void ScreenUpdateLine(int line);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_IBM_C) || defined(MAIN_VME_C))
extern void ScreenClear(void);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C) || defined(MAIN_C))
extern const char help_vme[];
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C) || defined(MAIN_C))
extern errr init_vme(int argc, char **argv);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern getkey(void);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern getkeybuf(void);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern char *cons;
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern int CNSINTR;
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern void InitConsole(void);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern void TerminateConsole(void);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern void ResetScrBuf(void);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern void AddScrBuf(char * ptr, int len);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern void GetAddr(int y, int x, char *stream);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern char InKey(void);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern char InKeyBuf(void);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern void ResetDISP(void);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern int kbhit(void);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern void ShowLine(int y, int x, int len);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern void LoadProfile(void);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VCS_C) || defined(MAIN_VME_C) || defined(UTIL_C))
extern open(char *name, int flags, int mode);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VCS_C) || defined(MAIN_VME_C) || defined(UTIL_C))
extern close(int fd);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_CAP_C) || defined(MAIN_GCU_C) || defined(MAIN_LSL_C) || defined(MAIN_ROS_C) || defined(MAIN_VCS_C) || defined(MAIN_VME_C) || defined(UTIL_C))
extern read(int fd, char *buff, int bytes);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_CAP_C) || defined(MAIN_DOS_C) || defined(MAIN_GCU_C) || defined(MAIN_IBM_C) || defined(MAIN_ROS_C) || defined(MAIN_VCS_C) || defined(MAIN_VME_C) || defined(UTIL_C))
extern write(int fd, char *buff, int bytes);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VCS_C) || defined(MAIN_VME_C) || defined(UTIL_C))
extern lseek(int fd, long pos, int set);
#endif
#if ((defined(USE_VME) || defined(VM))) && (defined(MAIN_VME_C))
extern unlink(char *filename);
#endif

/* main-win.c */

#if (defined(WINDOWS)) && (defined(MAIN_CRB_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C))
extern bool game_in_progress;
#endif
#if (defined(WINDOWS)) && (defined(MAIN_CRB_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(Z_RAND_C))
extern bool initialized;
#endif
#if (defined(WINDOWS)) && (defined(MAIN_WIN_C))
extern bool paletted;
#endif
#if (defined(WINDOWS)) && (defined(MAIN_WIN_C))
extern bool colors16;
#endif
#if ((defined(WINDOWS)) && defined(USE_SAVER)) && (defined(MAIN_WIN_C))
extern LRESULT FAR PASCAL AngbandSaverProc(HWND hWnd, UINT uMsg,
                                            WPARAM wParam, LPARAM lParam)
;
#endif
#if (defined(WINDOWS)) && (defined(MAIN_WIN_C))
extern int FAR PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
                       LPSTR lpCmdLine, int nCmdShow)
;
#endif

/* main-x11.c */

#if (defined(USE_X11)) && (defined(MAIN_X11_C) || defined(MAIN_C))
extern const char help_x11[];
#endif
#if (defined(USE_X11)) && (defined(MAIN_X11_C) || defined(MAIN_C))
extern errr init_x11(int argc, char **argv);
#endif

/* main-xaw.c */

#if (defined(USE_XAW)) && (defined(MAIN_XAW_C) || defined(MAIN_C))
extern const char help_xaw[];
#endif
#if (defined(USE_XAW)) && (defined(MAIN_XAW_C) || defined(MAIN_C))
extern errr init_xaw(int argc, char **argv);
#endif

/* main-xpj.c */

#if (defined(USE_XPJ)) && (defined(MAIN_XPJ_C) || defined(MAIN_C))
extern const char help_xpj[];
#endif
#if (defined(USE_XPJ)) && (defined(MAIN_XPJ_C) || defined(MAIN_C))
extern errr init_xpj(int argc, char **argv);
#endif

/* main-xxx.c */

#if (defined(USE_XXX)) && (defined(MAIN_XXX_C))
extern cptr help_xxx;
#endif
#if (defined(USE_XXX)) && (defined(MAIN_XXX_C))
extern errr init_xxx(int argc, char **argv);
#endif

/* melee1.c */

#if (defined(CMD2_C) || defined(MELEE1_C))
extern int check_hit(int power, int level);
#endif
#if (defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER1_C) || defined(WIZARD1_C))
extern blow_method_type *get_blow_method(byte idx);
#endif
#if (defined(MELEE1_C) || defined(MELEE2_C))
extern bool make_attack_normal(int m_idx);
#endif

/* melee2.c */

#if (defined(MELEE2_C) || defined(XTRA2_C))
extern void curse_equipment(int chance, int heavy_chance);
#endif
#if (defined(DUNGEON_C) || defined(MELEE2_C))
extern void process_monsters(void);
#endif

/* monster1.c */

#if (defined(MONSTER1_C) || defined(WIZARD1_C))
extern cptr describe_death_events(int r_idx, cptr he, bool omniscient);
#endif
#if (defined(MONSTER1_C) || defined(WIZARD1_C))
extern cptr roff_monster(u32b flags2, u32b flags3);
#endif
#if (defined(CMD3_C) || defined(MONSTER1_C))
extern void roff_top(int r_idx);
#endif
#if (defined(CMD3_C) || defined(MONSTER1_C) || defined(XTRA2_C))
extern void screen_roff(int r_idx);
#endif
#if (defined(MONSTER1_C) || defined(XTRA1_C))
extern void display_roff(int r_idx);
#endif

/* monster2.c */

#if (defined(GENERATE_C) || defined(MONSTER2_C))
extern bool place_ghost(void);
#endif
#if (defined(CMD1_C) || defined(LOAD_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern void delete_monster_idx(int i,bool visibly);
#endif
#if (defined(GENERATE_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS2_C))
extern void delete_monster(int y, int x);
#endif
#if (defined(LOAD_C) || defined(MONSTER2_C))
extern bool grow_m_list(void);
#endif
#if (defined(DUNGEON_C) || defined(MONSTER2_C) || defined(SAVE_C))
extern void compact_monsters(int size);
#endif
#if (defined(GENERATE_C) || defined(MONSTER2_C))
extern void remove_non_pets(void);
#endif
#if (defined(LOAD_C) || defined(MONSTER2_C))
extern s16b m_pop(void);
#endif
#if (defined(DUNGEON_C) || defined(GENERATE_C) || defined(MONSTER2_C))
extern void get_mon_num_prep(bool (*hook)(int, int), int p);
#endif
#if (defined(GENERATE_C) || defined(MONSTER2_C) || defined(SPELLS1_C))
extern s16b get_mon_num(int level);
#endif
#if (defined(CMD1_C) || defined(CMD3_C) || defined(CMD4_C) || defined(GENERATE_C) || defined(INIT1_C) || defined(MONSTER1_C) || defined(MONSTER2_C) || defined(QUEST_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA1_C))
extern void monster_desc_aux_f3(char *buf, uint max, cptr fmt, va_list *vp);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD4_C) || defined(GENERATE_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void monster_desc_f2(char *buf, uint max, cptr fmt, va_list *vp);
#endif
#if (defined(MONSTER2_C) || defined(SPELLS2_C))
extern void lore_do_probe(int m_idx);
#endif
#if (defined(MONSTER2_C) || defined(XTRA2_C))
extern void lore_treasure(int m_idx, int num_item, int num_gold);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(SPELLS2_C))
extern void update_mon(int m_idx, bool full);
#endif
#if (defined(MONSTER2_C) || defined(XTRA1_C))
extern void update_monsters(bool full);
#endif
#if (defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern bool live_monster_p(monster_race *r_ptr);
#endif
#if (defined(CMD1_C) || defined(MONSTER2_C) || defined(SPELLS1_C))
extern bool live_monster_wide_p(monster_race *r_ptr);
#endif
#if (defined(MONSTER2_C) || defined(XTRA2_C))
extern bool place_monster_one(int y, int x, int r_idx, bool slp, bool charm, bool force);
#endif
#if (defined(CMD1_C) || defined(GENERATE_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(WIZARD2_C))
extern bool place_monster_aux(int y, int x, int r_idx, bool slp, bool grp, bool charm, bool force);
#endif
#if (defined(GENERATE_C) || defined(MONSTER2_C))
extern bool place_monster(int y, int x, int level, bool slp, bool grp);
#endif
#if (defined(GENERATE_C) || defined(MONSTER2_C))
extern bool put_quest_monster(int r_idx);
#endif
#if (defined(MONSTER_HORDES)) && (defined(MONSTER2_C) || defined(WIZARD2_C))
extern bool alloc_horde(int y, int x, int level);
#endif
#if (defined(DUNGEON_C) || defined(GENERATE_C) || defined(MONSTER2_C))
extern void alloc_monster(int dis, int level, bool slp);
#endif
#if (defined(DUNGEON_C) || defined(MELEE2_C) || defined(MONSTER2_C))
extern bool summon_specific_aux(int y1, int x1, int lev, int type, bool Group_ok, bool charm);
#endif
#if (defined(CMD2_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern bool summon_specific(int y1, int x1, int lev, int type);
#endif
#if (defined(MELEE2_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(XTRA2_C))
extern bool summon_specific_friendly(int y1, int x1, int lev, int type, bool Group_ok);
#endif
#if (defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C))
extern bool multiply_monster(int m_idx, bool charm, bool clone);
#endif
#if (defined(CMD1_C) || defined(MONSTER2_C) || defined(SPELLS1_C))
extern void message_pain(int m_idx, int dam);
#endif
#if (defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C))
extern void update_smart_learn(int m_idx, int what);
#endif

/* object1.c */

#if (defined(FILES_C) || defined(OBJECT1_C))
extern s16b lookup_unident(byte p_id, byte s_id);
#endif
#if (defined(DUNGEON_C) || defined(OBJECT1_C))
extern void flavor_init(void);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(OBJECT1_C))
extern void reset_visuals(void);
#endif
#if (defined(CMD1_C) || defined(CMD3_C) || defined(DUNGEON_C) || defined(LOAD_C) || defined(MELEE2_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA1_C))
extern void object_flags(object_ctype *o_ptr, u32b *f1, u32b *f2, u32b *f3);
#endif
#if (defined(CMD4_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MELEE2_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SQUELCH_C))
extern void object_info_known(object_type *j_ptr, object_ctype *o_ptr);
#endif
#if (defined(FILES_C) || defined(OBJECT1_C))
extern void object_flags_known(object_ctype *o_ptr, u32b *f1, u32b *f2, u32b *f3);
#endif
#if (defined(MONSTER2_C) || defined(OBJECT1_C) || defined(XTRA2_C))
extern cptr find_next_good_flag(cptr s, byte reject, byte require);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER1_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void object_desc_f3(char *buf, uint max, cptr fmt, va_list *vp);
#endif
#if (defined(CMD4_C) || defined(OBJECT1_C) || defined(SQUELCH_C) || defined(WIZARD2_C))
extern void object_k_name_f1(char *buf, uint max, cptr UNUSED fmt, va_list *vp);
#endif
#if (defined(OBJECT1_C) || defined(WIZARD1_C))
extern cptr PURE item_activation(object_ctype *o_ptr);
#endif
#if (defined(OBJECT1_C) || defined(POWERS_C))
extern cptr list_flags(cptr init, cptr conj, cptr *flags, int total);
#endif
#if (defined(FILES_C) || defined(OBJECT1_C))
extern object_ctype PURE *get_real_obj(object_ctype *o_ptr);
#endif
#if (defined(CMD6_C) || defined(OBJECT1_C) || defined(STORE_C))
extern bool PURE is_worn_p(object_ctype *o_ptr);
#endif
#if (defined(CMD6_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(SPELLS2_C))
extern bool PURE is_inventory_p(object_ctype *o_ptr);
#endif
#if (defined(CMD1_C) || defined(FILES_C) || defined(OBJECT1_C))
extern int PURE get_bow_mult(object_ctype *o_ptr);
#endif
#if (defined(FILES_C) || defined(OBJECT1_C))
extern s16b PURE launcher_type(object_ctype *o_ptr);
#endif
#if (defined(FILES_C) || defined(OBJECT1_C) || defined(XTRA1_C))
extern byte PURE ammunition_type(object_ctype *o_ptr);
#endif
#if (defined(CMD3_C) || defined(OBJECT1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(XTRA1_C))
extern bool identify_fully_aux(object_ctype *o_ptr, byte flags);
#endif
#if (defined(FILES_C) || defined(OBJECT1_C))
extern void identify_fully_file(object_ctype *o_ptr, FILE *fff);
#endif
#if (defined(CMD2_C) || defined(CMD3_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MELEE1_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C))
extern s16b PURE index_to_label(object_ctype *o_ptr);
#endif
#if (defined(CMD3_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(XTRA1_C))
extern s16b PURE wield_slot(object_ctype *o_ptr);
#endif
#if (defined(CMD3_C) || defined(DUNGEON_C) || defined(OBJECT1_C) || defined(SPELLS2_C))
extern cptr PURE describe_use(object_ctype *o_ptr);
#endif
#if (defined(CMD3_C) || defined(OBJECT1_C))
extern bool item_tester_okay(object_ctype *o_ptr);
#endif
#if (defined(OBJECT1_C) || defined(XTRA1_C))
extern void display_inven(bool equip);
#endif



#if (defined(CMD3_C) || defined(FILES_C) || defined(OBJECT1_C))
extern void show_inven(bool equip, bool all);
#endif



#if (defined(OBJECT1_C) || defined(SQUELCH_C))
extern void next_object(object_type **o_ptr);
#endif
#if (defined(DUNGEON_C) || defined(OBJECT1_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(STORE_C))
extern object_type *get_item(errr *err, cptr pmt, bool equip, bool inven, bool floor);
#endif

/* object2.c */

#if (defined(LOAD_C) || defined(MELEE2_C) || defined(OBJECT2_C))
extern void excise_dun_object(object_type *j_ptr);
#endif
#if (defined(CMD2_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(XTRA2_C))
extern void delete_dun_object(object_type *j_ptr);
#endif
#if (defined(DUNGEON_C) || defined(GENERATE_C) || defined(OBJECT2_C) || defined(SPELLS2_C))
extern void delete_object(int y, int x);
#endif
#if (defined(DUNGEON_C) || defined(OBJECT2_C))
extern void do_cmd_rotate_stack(void);
#endif
#if (defined(LOAD_C) || defined(OBJECT2_C))
extern bool grow_o_list(void);
#endif
#if (defined(DUNGEON_C) || defined(OBJECT2_C) || defined(SAVE_C))
extern void compact_objects(int size);
#endif
#if (defined(GENERATE_C) || defined(OBJECT2_C))
extern void wipe_o_list(bool preserve);
#endif
#if (defined(LOAD_C) || defined(MELEE1_C) || defined(OBJECT2_C))
extern object_type *o_pop(void);
#endif
#if (defined(OBJECT2_C) || defined(STORE_C))
extern s16b get_obj_num(int level);
#endif
#if (defined(BIRTH_C) || defined(CMD2_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C))
extern void object_known(object_type *o_ptr);
#endif
#if (defined(BIRTH_C) || defined(CMD6_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD2_C))
extern void object_aware(object_type *o_ptr);
#endif
#if (defined(CMD6_C) || defined(OBJECT2_C))
extern void object_tried(object_type *o_ptr);
#endif
#if (defined(OBJECT2_C) || defined(SPELLS2_C))
extern s32b PURE flag_cost(object_ctype *o_ptr, bool all);
#endif
#if (defined(CMD3_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD1_C) || defined(WIZARD2_C))
extern s32b PURE object_value(object_ctype *o1_ptr, bool full);
#endif
#if (defined(MELEE1_C) || defined(OBJECT2_C) || defined(SPELLS2_C) || defined(XTRA1_C))
extern void set_stack_number(object_type *o_ptr);
#endif
#if (defined(OBJECT2_C) || defined(STORE_C))
extern int PURE object_similar_2(object_ctype *o_ptr, object_ctype *j_ptr);
#endif
#if (defined(OBJECT2_C) || defined(STORE_C))
extern bool PURE object_similar(object_ctype *o_ptr, object_ctype *j_ptr);
#endif
#if (defined(OBJECT2_C) || defined(STORE_C))
extern bool object_absorb(object_type *o_ptr, object_type *j_ptr);
#endif
#if (defined(BIRTH_C) || defined(CMD2_C) || defined(FILES_C) || defined(LOAD_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern void object_wipe(object_type *o_ptr);
#endif
#if (defined(CMD1_C) || defined(CMD3_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(LOAD_C) || defined(MELEE1_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern void object_copy(object_type *o_ptr, object_ctype *j_ptr);
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MONSTER1_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(STORE_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern void object_prep(object_type *o_ptr, int k_idx);
#endif
#if (defined(CHECK_ARRAYS)) && (defined(INIT2_C) || defined(OBJECT2_C))
extern void check_bonus_table(void);
#endif
#if (defined(CMD6_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern void apply_magic_2(object_type *o_ptr, const int lev);
#endif
#if (defined(OBJECT2_C) || defined(STORE_C) || defined(WIZARD2_C))
extern void apply_magic(object_type *o_ptr, int lev, bool okay, bool good, bool great);
#endif
#if (defined(BIRTH_C) || defined(OBJECT2_C) || defined(XTRA2_C))
extern void make_item(object_type *o_ptr, make_item_type *i_ptr);
#endif
#if (defined(DUNGEON_C) || defined(OBJECT2_C))
extern bool PURE magic_can_curse(int k_idx);
#endif
#if (defined(CMD2_C) || defined(OBJECT2_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern bool make_object(object_type *j_ptr, bool good, bool great);
#endif
#if (defined(CMD2_C) || defined(GENERATE_C) || defined(OBJECT2_C) || defined(SPELLS1_C))
extern void place_object(int y, int x, bool good, bool great);
#endif
#if (defined(CMD2_C) || defined(OBJECT2_C) || defined(XTRA2_C))
extern bool make_gold(object_type *j_ptr);
#endif
#if (defined(CMD2_C) || defined(GENERATE_C) || defined(OBJECT2_C) || defined(SPELLS1_C))
extern void place_gold(int y, int x);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern object_type *drop_near(object_type *j_ptr, int chance, int y, int x);
#endif
#if (defined(DUNGEON_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(XTRA2_C))
extern void acquirement(int y1, int x1, int num, bool great);
#endif
#if (defined(CMD2_C) || defined(OBJECT2_C) || defined(SPELLS2_C))
extern void pick_trap(int y, int x);
#endif
#if (defined(GENERATE_C) || defined(OBJECT2_C) || defined(SPELLS1_C))
extern void place_trap(int y, int x);
#endif
#if (defined(CMD6_C) || defined(OBJECT2_C))
extern void item_charges(object_ctype *o_ptr);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C))
extern void item_increase(object_type *o_ptr, int num);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(OBJECT2_C) || defined(SPELLS2_C) || defined(STORE_C))
extern void item_describe(object_ctype *o_ptr);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C))
extern void item_optimize(object_type *o_ptr);
#endif
#if (defined(CMD2_C) || defined(OBJECT2_C) || defined(STORE_C))
extern bool PURE inven_carry_okay(object_ctype *o_ptr);
#endif
#if (defined(BIRTH_C) || defined(CMD2_C) || defined(CMD6_C) || defined(OBJECT2_C) || defined(STORE_C))
extern object_type *inven_carry(object_type *o_ptr);
#endif
#if (defined(CMD3_C) || defined(OBJECT2_C))
extern object_type *inven_takeoff(object_type *o_ptr, int amt);
#endif
#if (defined(CMD3_C) || defined(DUNGEON_C) || defined(OBJECT2_C))
extern void inven_drop(object_type *o_ptr, int amt);
#endif
#if (defined(OBJECT2_C) || defined(XTRA1_C))
extern void combine_pack(void);
#endif
#if (defined(OBJECT2_C) || defined(XTRA1_C))
extern void reorder_pack(void);
#endif
#if (defined(OBJECT2_C) || defined(XTRA1_C))
extern void display_koff(int k_idx);
#endif
#if (defined(CMD3_C) || defined(OBJECT2_C))
extern void object_hide(object_type *o_ptr);
#endif

/* powers.c */

#if (defined(CMD5_C) || defined(CMD6_C) || defined(POWERS_C))
extern cptr describe_power(int power, int lev);
#endif
#if (defined(DUNGEON_C) || defined(POWERS_C) || defined(XTRA2_C))
extern void do_poly_wounds(int cause);
#endif
#if (defined(POWERS_C) || defined(SPELLS1_C) || defined(XTRA2_C))
extern void do_poly_self(void);
#endif
#if (defined(CMD5_C) || defined(POWERS_C))
extern void wild_magic(int spell);
#endif
#if (defined(CMD6_C) || defined(POWERS_C))
extern bool use_object_power(const int power, int dir, bool *ident, bool *use);
#endif
#if (defined(CMD1_C) || defined(CMD5_C) || defined(POWERS_C))
extern bool use_known_power(int power, int plev);
#endif

/* quest.c */

#if (defined(GENERATE_C) || defined(MONSTER2_C) || defined(QUEST_C) || defined(XTRA2_C))
extern quest_type *get_quest(void);
#endif
#if (defined(GENERATE_C) || defined(QUEST_C))
extern int get_quest_monster(void);
#endif
#if (defined(CMD4_C) || defined(QUEST_C))
extern void print_quest_message(void);
#endif
#if (defined(DUNGEON_C) || defined(QUEST_C))
extern void quest_discovery(bool new);
#endif
#if (defined(DUNGEON_C) || defined(QUEST_C))
extern void set_guardians(void);
#endif
#if (defined(CMD4_C) || defined(QUEST_C))
extern quest_type *cnv_monster_to_quest(monster_race *r_ptr);
#endif

/* readdib.c */

#if ((defined(WINDOWS) && defined(USE_GRAPHICS))) && (defined(READDIB_H) || defined(MAIN_WIN_C) || defined(READDIB_C))
extern BOOL ReadDIB(HWND hWnd, LPSTR lpFileName, DIBINIT *pInfo);
#endif

/* save.c */

#if (defined(FILES_C) || defined(MAIN_CRB_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(SAVE_C))
extern bool save_player(bool as_4_1_0);
#endif

/* spells1.c */

#if (defined(CMD1_C) || defined(SPELLS1_C))
extern s16b poly_r_idx(int r_idx);
#endif
#if (defined(CMD1_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(SPELLS1_C))
extern void teleport_away(int m_idx, int dis);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C) || defined(MELEE2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C))
extern void teleport_player(int dis);
#endif
#if (defined(MELEE2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(WIZARD2_C))
extern void teleport_player_to(int ny, int nx);
#endif
#if (defined(MELEE2_C) || defined(POWERS_C) || defined(SPELLS1_C))
extern void teleport_player_level(void);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern void take_hit(int damage, cptr hit_from, int monster);
#endif
#if (defined(CMD2_C) || defined(MELEE1_C) || defined(SPELLS1_C))
extern void acid_dam(int dam, cptr kb_str, int monster);
#endif
#if (defined(MELEE1_C) || defined(SPELLS1_C))
extern void elec_dam(int dam, cptr kb_str, int monster);
#endif
#if (defined(CMD2_C) || defined(MELEE1_C) || defined(SPELLS1_C))
extern void fire_dam(int dam, cptr kb_str, int monster);
#endif
#if (defined(MELEE1_C) || defined(SPELLS1_C))
extern void cold_dam(int dam, cptr kb_str, int monster);
#endif
#if (defined(SPELLS1_C) || defined(SPELLS2_C))
extern bool inc_stat(int stat);
#endif
#if (defined(CMD5_C) || defined(DUNGEON_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(XTRA2_C))
extern bool dec_stat(int stat, int amount, int permanent);
#endif
#if (defined(OBJECT1_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(WIZARD2_C))
extern bool res_stat(int stat);
#endif
#if (defined(MELEE1_C) || defined(POWERS_C) || defined(SPELLS1_C))
extern bool apply_disenchant(int mode);
#endif
#if (defined(POWERS_C) || defined(SPELLS1_C))
extern void chaos_feature_shuffle(void);
#endif
#if (defined(CMD1_C) || defined(CMD5_C) || defined(MELEE2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern bool project(monster_type *mw_ptr, int rad, int y, int x, int dam, int typ, int flg);
#endif
#if (defined(CMD6_C) || defined(OBJECT2_C) || defined(SPELLS1_C))
extern void potion_smash_effect(monster_type *m_ptr, int y, int x, int o_kidx);
#endif

/* spells2.c */

#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool dimension_door(int plev, int fail_dis);
#endif
#if (defined(CMD1_C) || defined(DUNGEON_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern bool hp_player(int num);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern void warding_glyph(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern void explosive_rune(void);
#endif
#if (defined(CMD2_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern bool do_dec_stat(int stat);
#endif
#if (defined(SPELLS1_C) || defined(SPELLS2_C))
extern void do_dec_stat_time(int stat, bool msg);
#endif
#if (defined(DUNGEON_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(XTRA2_C))
extern bool do_res_stat(int stat);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern bool do_inc_stat(int stat);
#endif
#if (defined(DUNGEON_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(STORE_C))
extern void identify_pack(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool remove_curse(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C) || defined(WIZARD2_C))
extern bool remove_all_curse(void);
#endif
#if (defined(DUNGEON_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern bool restore_level(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool alchemy(void);
#endif
#if (defined(DUNGEON_C) || defined(POWERS_C) || defined(SPELLS2_C))
extern void self_knowledge(void);
#endif
#if (defined(CMD5_C) || defined(DUNGEON_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(SPELLS2_C))
extern bool lose_all_info(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern PURE bool detect_traps_p(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool detect_traps(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool detect_doors(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool detect_stairs(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool detect_treasure(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool detect_objects_gold(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool detect_objects_normal(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool detect_objects_magic(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool detect_monsters_normal(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool detect_monsters_invis(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool detect_monsters_evil(void);
#endif
#if (defined(DUNGEON_C) || defined(POWERS_C) || defined(SPELLS2_C))
extern bool detect_all(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern void stair_creation(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool PURE item_tester_hook_armour(object_ctype *o_ptr);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool enchant(object_type *o_ptr, int n, int eflag);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C) || defined(STORE_C))
extern bool enchant_spell(int num_hit, int num_dam, int num_ac);
#endif
#if (defined(OBJECT2_C) || defined(SPELLS2_C))
extern void random_resistance (object_type * o_ptr, bool is_scroll, int specific);
#endif
#if (defined(OBJECT2_C) || defined(SPELLS2_C))
extern bool create_artifact(object_type *o_ptr, bool a_scroll);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool artifact_scroll(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool ident_spell(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool identify_fully(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool PURE item_tester_hook_recharge(object_ctype *o_ptr);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool recharge(int num);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool speed_monsters(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool slow_monsters(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool sleep_monsters(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool banish_evil(int dist);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool dispel_undead(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool dispel_evil(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool dispel_good(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern bool dispel_monsters(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool dispel_living(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool dispel_demons(int dam);
#endif
#if (defined(MELEE2_C) || defined(POWERS_C) || defined(SPELLS2_C))
extern void aggravate_monsters(int who);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern errr genocide(bool player_cast);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern errr mass_genocide(bool player_cast);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool probing(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern void destroy_area(int y1, int x1, int r, bool full);
#endif
#if (defined(CMD1_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(POWERS_C) || defined(SPELLS2_C))
extern void earthquake(int cy, int cx, int r);
#endif
#if (defined(MELEE2_C) || defined(SPELLS2_C))
extern void unlite_room(int y1, int x1);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool lite_area(int dam, int rad);
#endif
#if (defined(DUNGEON_C) || defined(MELEE2_C) || defined(POWERS_C) || defined(SPELLS2_C))
extern bool unlite_area(int dam, int rad);
#endif
#if (defined(DUNGEON_C) || defined(MELEE2_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern bool fire_ball(int typ, int dir, int dam, int rad);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool fire_bolt(int typ, int dir, int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool fire_beam(int typ, int dir, int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool fire_bolt_or_beam(int prob, int typ, int dir, int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool lite_line(int dir);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool wall_to_mud(int dir);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool wizard_lock(int dir);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool destroy_door(int dir);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool disarm_trap(int dir);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool door_creation(void);
#endif
#if (defined(MELEE2_C) || defined(POWERS_C) || defined(SPELLS2_C))
extern bool trap_creation(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool glyph_creation(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool wall_stone(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool destroy_doors_touch(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern bool sleep_monsters_touch(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern void call_chaos(int plev);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern void activate_ty_curse(void);
#endif
#if (defined(CMD2_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern void activate_hi_summon(void);
#endif
#if (defined(MELEE2_C) || defined(POWERS_C) || defined(SPELLS2_C))
extern void summon_reaver(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern void wall_breaker(int plev);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern void bless_weapon(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern  bool detect_monsters_nonliving(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern void set_recall(bool spell);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern  bool confuse_monsters(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern  bool charm_monsters(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern  bool charm_animals(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern  bool stun_monsters(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern  bool stasis_monsters(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern  bool mindblast_monsters(int dam);
#endif
#if (defined(DUNGEON_C) || defined(POWERS_C) || defined(SPELLS2_C))
extern  bool banish_monsters(int dist);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern  bool turn_evil(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern  bool turn_monsters(int dam);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern  bool charm_monster(int dir, int plev);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern  bool control_one_undead(int dir, int plev);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern  bool charm_animal(int dir, int plev);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern void report_magics(void);
#endif
#if (defined(POWERS_C) || defined(SPELLS2_C))
extern void teleport_swap(int dir);
#endif
#if (defined(DUNGEON_C) || defined(SPELLS2_C))
extern void alter_reality(void);
#endif

/* squelch.c */

#if (defined(CMD4_C) || defined(SQUELCH_C))
extern void inscription_dump(FILE *fff);
#endif
#if (defined(FILES_C) || defined(SQUELCH_C))
extern cptr process_pref_squelch(char **zz, int n, u16b *sf_flags);
#endif
#if (defined(CMD4_C) || defined(SQUELCH_C))
extern void squelch_dump(FILE *fff);
#endif
#if (defined(SQUELCH_C) || defined(WIZARD2_C))
extern void get_names(char **obuf, char *this, int num, int *choice, uint len,
	void (*print_f1)(char *, uint, cptr, va_list *))
;
#endif
#if (defined(SQUELCH_C) || defined(WIZARD2_C))
extern name_centry *choose_item_category(bool (*item_good)(int, name_centry *),
	bool *abort, name_centry *start, cptr prompt, bool squelch)
;
#endif
#if (defined(CMD4_C) || defined(SQUELCH_C))
extern void do_cmd_options_squelch(void);
#endif
#if (defined(SQUELCH_C) || defined(XTRA1_C))
extern void squelch_grid(void);
#endif
#if (defined(SQUELCH_C) || defined(XTRA1_C))
extern void squelch_inventory(void);
#endif

/* store.c */

#if (defined(OBJECT2_C) || defined(STORE_C))
extern bool store_object_absorb(object_type *j_ptr, object_type *o_ptr);
#endif
#if (defined(CMD4_C) || defined(STORE_C))
extern cptr store_title(int store_num);
#endif
#if (defined(SAVE_C) || defined(STORE_C))
extern store_type *find_house(int town);
#endif
#if (defined(DUNGEON_C) || defined(STORE_C))
extern void do_cmd_store(void);
#endif
#if (defined(DUNGEON_C) || defined(LOAD_C) || defined(STORE_C))
extern bool store_shuffle(int which);
#endif
#if (defined(BIRTH_C) || defined(DUNGEON_C) || defined(STORE_C))
extern void store_maint(int which);
#endif
#if (defined(BIRTH_C) || defined(STORE_C))
extern void store_init(int which);
#endif

/* tables.c */

#if (defined(GENERATE_C) || defined(MELEE2_C) || defined(POWERS_C) || defined(TABLES_C) || defined(XTRA2_C))
extern s16b ddd[9];
#endif
#if (defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(GENERATE_C) || defined(MELEE2_C) || defined(POWERS_C) || defined(TABLES_C) || defined(XTRA2_C))
extern s16b ddx[10];
#endif
#if (defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(GENERATE_C) || defined(MELEE2_C) || defined(POWERS_C) || defined(TABLES_C) || defined(XTRA2_C))
extern s16b ddy[10];
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(GENERATE_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(TABLES_C))
extern s16b ddx_ddd[9];
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(GENERATE_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(TABLES_C))
extern s16b ddy_ddd[9];
#endif
#if (defined(MAIN_DOS_C) || defined(MAIN_EMX_C) || defined(MAIN_IBM_C) || defined(MAIN_VCS_C) || defined(MAIN_WIN_C) || defined(TABLES_C) || defined(UTIL_C))
extern char hexsym[16];
#endif
#if (defined(BIRTH_C) || defined(CMD5_C) || defined(OBJECT1_C) || defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_mag_study[NUM_STAT_INDICES];
#endif
#if (defined(BIRTH_C) || defined(OBJECT1_C) || defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_mag_mana[NUM_STAT_INDICES];
#endif
#if (defined(BIRTH_C) || defined(CMD5_C) || defined(OBJECT1_C) || defined(TABLES_C))
extern const byte adj_mag_fail[NUM_STAT_INDICES];
#endif
#if (defined(CMD5_C) || defined(OBJECT1_C) || defined(TABLES_C))
extern const byte adj_mag_stat[NUM_STAT_INDICES];
#endif
#if (defined(OBJECT1_C) || defined(STORE_C) || defined(TABLES_C))
extern const byte adj_chr_gold[NUM_STAT_INDICES];
#endif
#if (defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_int_dev[NUM_STAT_INDICES];
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_wis_sav[NUM_STAT_INDICES];
#endif
#if (defined(BIRTH_C) || defined(OBJECT1_C) || defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_dex_dis[NUM_STAT_INDICES];
#endif
#if (defined(BIRTH_C) || defined(OBJECT1_C) || defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_int_dis[NUM_STAT_INDICES];
#endif
#if (defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_dex_ta[NUM_STAT_INDICES];
#endif
#if (defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_str_td[NUM_STAT_INDICES];
#endif
#if (defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_dex_th[NUM_STAT_INDICES];
#endif
#if (defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_str_th[NUM_STAT_INDICES];
#endif
#if (defined(BIRTH_C) || defined(CMD3_C) || defined(OBJECT1_C) || defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_str_wgt[NUM_STAT_INDICES];
#endif
#if (defined(BIRTH_C) || defined(OBJECT1_C) || defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_str_hold[NUM_STAT_INDICES];
#endif
#if (defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_str_dig[NUM_STAT_INDICES];
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(OBJECT1_C) || defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_str_blow[NUM_STAT_INDICES];
#endif
#if (defined(OBJECT1_C) || defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_dex_blow[NUM_STAT_INDICES];
#endif
#if (defined(BIRTH_C) || defined(CMD2_C) || defined(CMD4_C) || defined(MELEE1_C) || defined(OBJECT1_C) || defined(TABLES_C))
extern const byte adj_dex_safe[NUM_STAT_INDICES];
#endif
#if (defined(BIRTH_C) || defined(DUNGEON_C) || defined(OBJECT1_C) || defined(SPELLS1_C) || defined(TABLES_C))
extern const byte adj_con_fix[NUM_STAT_INDICES];
#endif
#if (defined(OBJECT1_C) || defined(TABLES_C) || defined(XTRA1_C))
extern const byte adj_con_mhp[NUM_STAT_INDICES];
#endif
#if (defined(OBJECT1_C) || defined(TABLES_C) || defined(XTRA1_C))
extern byte blows_table[12][12];
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER1_C) || defined(MONSTER2_C) || defined(TABLES_C))
extern u16b extract_energy[200];
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(LOAD_C) || defined(TABLES_C))
extern player_sex sex_info[MAX_SEXES];
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(INIT1_C) || defined(LOAD_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(STORE_C) || defined(TABLES_C))
extern player_race race_info[MAX_RACES];
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(LOAD_C) || defined(TABLES_C))
extern player_template template_info[MAX_TEMPLATE];
#endif
#if (defined(CMD5_C) || defined(INIT2_C) || defined(TABLES_C) || defined(XTRA1_C))
extern book_type book_info[MAX_BK];
#endif
#if (defined(BIRTH_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD4_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(TABLES_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern  player_skill skill_set[MAX_SKILLS];
#endif
#if (defined(CMD2_C) || defined(OBJECT1_C) || defined(TABLES_C))
extern byte chest_traps[64];
#endif
#if (defined(CMD4_C) || defined(MAIN_AMI_C) || defined(TABLES_C))
extern cptr color_names[16];
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(TABLES_C) || defined(UTIL_C) || defined(XTRA1_C))
extern cptr atchar;
#endif
#if (defined(CMD1_C) || defined(FILES_C) || defined(TABLES_C) || defined(WIZARD2_C) || defined(XTRA1_C))
extern cptr stat_names[6];
#endif
#if (defined(FILES_C) || defined(TABLES_C) || defined(XTRA1_C))
extern cptr stat_names_reduced[6];
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(SAVE_C) || defined(TABLES_C))
extern option_type option_info[];
#endif
#if (defined(CMD4_C) || defined(TABLES_C))
extern force_type option_force[];
#endif
#if (defined(TABLES_C) || defined(XTRA2_C))
extern cptr chaos_patron_shorts[MAX_PATRON];
#endif
#if (defined(TABLES_C) || defined(XTRA2_C))
extern int chaos_stats[MAX_PATRON];
#endif
#if (defined(TABLES_C) || defined(XTRA2_C))
extern int chaos_rewards[MAX_PATRON][20];
#endif
#if (defined(CMD1_C) || defined(FILES_C) || defined(INIT2_C) || defined(TABLES_C))
extern martial_arts ma_blows[MAX_MA+1];
#endif
#if (defined(ANGBAND_H))
extern window_type windows[8];
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(MONSTER1_C) || defined(OBJECT1_C) || defined(TABLES_C))
extern moncol_type moncol[MAX_MONCOL];
#endif
#if (defined(ANGBAND_H))
extern wild_type wild_grid[12][12];
#endif
#if (defined(BIRTH_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(SAVE_C) || defined(STORE_C) || defined(TABLES_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern spirit_type spirits[MAX_SPIRITS];
#endif
#if (defined(INIT1_C) || defined(MELEE1_C) || defined(TABLES_C))
extern blow_method_type blow_methods[NUM_BLOW_METHODS];
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(INIT2_C) || defined(TABLES_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern redraw_type screen_coords[NUM_SCREEN_COORDS];
#endif
#if (defined(INIT1_C) || defined(MONSTER1_C) || defined(POWERS_C) || defined(TABLES_C))
extern cptr explode_flags[];
#endif
#if (defined(INIT1_C) || defined(MONSTER1_C) || defined(TABLES_C))
extern cptr coin_types[];
#endif
#if (defined(CMD3_C) || defined(TABLES_C) || defined(WIZARD2_C))
extern name_centry ident_info[];
#endif
#if (defined(DUNGEON_C) || defined(INIT2_C) || defined(OBJECT1_C) || defined(TABLES_C))
extern cptr_ch feeling_str[SENSE_MAX];
#endif
#if (defined(SQUELCH_C) || defined(TABLES_C) || defined(WIZARD2_C))
extern cptr option_chars;
#endif
#if (defined(CMD1_C) || defined(FILES_C) || defined(TABLES_C))
extern natural_attack natural_attacks[5];
#endif

/* util.c */

#if (!(defined(HAS_MEMSET))) && (defined(ANGBAND_H) || defined(Z_VIRT_H))
extern char *memset(char *s, int c, huge n);
#endif
#if (!(defined(HAS_STRICMP))) && (defined(ANGBAND_H) || defined(H_CONFIG_H))
extern int stricmp(cptr a, cptr b);
#endif
#if ((defined(SET_UID)) && !(defined(HAS_USLEEP))) && (defined(MAIN_CAP_C) || defined(MAIN_GCU_C) || defined(MAIN_GTK_C) || defined(MAIN_LSL_C) || defined(MAIN_SLA_C) || defined(MAIN_VCS_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(UTIL_C))
extern int usleep(huge usecs);
#endif
#if (defined(SET_UID)) && (defined(MAIN_C) || defined(UTIL_C))
extern void user_name(char *buf, int id);
#endif
#if (!(defined(ACORN))) && (defined(FILES_C) || defined(LOAD_C) || defined(MAIN_ROS_C) || defined(SAVE_C) || defined(UTIL_C))
extern FILE *my_fopen(cptr file, cptr mode);
#endif
#if (!(defined(ACORN))) && (defined(BIRTH_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_XPJ_C) || defined(SAVE_C) || defined(SQUELCH_C) || defined(UTIL_C) || defined(WIZARD1_C))
extern errr my_fclose(FILE *fff);
#endif
#if (defined(FILES_C) || defined(INIT2_C) || defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_GTK_C) || defined(MAIN_IBM_C) || defined(MAIN_LSL_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(UTIL_C))
extern void path_build_f2(char *buf, uint max, cptr UNUSED fmt, va_list *vp);
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(MONSTER2_C) || defined(SQUELCH_C) || defined(UTIL_C) || defined(WIZARD1_C))
extern FILE *my_fopen_path(cptr path, cptr file, cptr mode);
#endif
#if (defined(HAVE_MKSTEMP)) && (defined(CMD4_C) || defined(FILES_C) || defined(UTIL_C))
extern FILE *my_fopen_temp(char *buf, uint max);
#endif
#if (!(defined(HAVE_MKSTEMP))) && (defined(CMD4_C) || defined(FILES_C) || defined(UTIL_C))
extern FILE *my_fopen_temp(char *buf, int max);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT1_C) || defined(INIT2_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_XPJ_C) || defined(UTIL_C))
extern errr my_fgets(FILE *fff, char *buf, size_t n);
#endif
#if (defined(FILES_C) || defined(UTIL_C))
extern errr my_fgets_long(char *buf, size_t n, FILE *fff);
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(SQUELCH_C) || defined(UTIL_C))
extern int my_fprintf(FILE *fff, cptr fmt, ...);
#endif
#if (!(defined(ACORN))) && (defined(CMD4_C) || defined(FILES_C) || defined(LOAD_C) || defined(MAIN_ROS_C) || defined(SAVE_C) || defined(UTIL_C) || defined(XTRA2_C))
extern errr fd_kill(cptr file);
#endif
#if (!(defined(ACORN))) && (defined(MAIN_ROS_C) || defined(SAVE_C) || defined(UTIL_C))
extern errr fd_move(cptr file, cptr what);
#endif
#if (!(defined(ACORN))) && (defined(INIT2_C) || defined(UTIL_C))
extern errr fd_copy(cptr out, cptr in);
#endif
#if ((!(defined(ACORN))) && defined(PRIVATE_USER_PATH)) && (defined(INIT2_C) || defined(UTIL_C))
extern errr my_mkdir(cptr path, uint mode);
#endif
#if (!(defined(ACORN))) && (defined(INIT2_C) || defined(MAIN_ROS_C) || defined(SAVE_C) || defined(UTIL_C))
extern int fd_make(cptr file, int mode);
#endif
#if (!(defined(ACORN))) && (defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MAIN_CRB_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(UTIL_C))
extern int fd_open(cptr file, int flags);
#endif
#if (!(defined(ACORN))) && (defined(FILES_C) || defined(MAIN_ROS_C) || defined(UTIL_C))
extern errr fd_lock(int fd, int what);
#endif
#if (!(defined(ACORN))) && (defined(FILES_C) || defined(MAIN_ROS_C) || defined(UTIL_C))
extern errr fd_seek(int fd, huge n);
#endif
#if (!(defined(ACORN))) && (defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MAIN_ROS_C) || defined(UTIL_C))
extern errr fd_read(int fd, char *buf, huge n);
#endif
#if (!(defined(ACORN))) && (defined(FILES_C) || defined(INIT2_C) || defined(MAIN_ROS_C) || defined(UTIL_C))
extern errr fd_write(int fd, cptr buf, huge n);
#endif
#if (!(defined(ACORN))) && (defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MAIN_CRB_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(SAVE_C) || defined(UTIL_C))
extern errr fd_close(int fd);
#endif
#if (defined(CAVE_C) || defined(CMD4_C) || defined(FILES_C) || defined(UTIL_C) || defined(WIZARD2_C))
extern void move_cursor(int row, int col);
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(UTIL_C))
extern void text_to_ascii_f1(char *buf, uint max, cptr UNUSED fmt, va_list *vp);
#endif
#if (defined(CMD4_C) || defined(UTIL_C))
extern void ascii_to_text_f1(char *buf, uint max, cptr UNUSED fmt, va_list *vp);
#endif
#if (defined(CMD4_C) || defined(MAIN_GTK_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(UTIL_C))
extern sint macro_find_exact(cptr pat);
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(MAIN_GTK_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(UTIL_C))
extern errr macro_add(cptr pat, cptr act);
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(CMD2_C) || defined(CMD4_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_CRB_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_X11_C) || defined(MAIN_XPJ_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(UTIL_C) || defined(WIZARD2_C))
extern void flush(void);
#endif
#if (defined(BIRTH_C) || defined(CMD1_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(CMD6_C) || defined(FILES_C) || defined(MAIN_X11_C) || defined(OBJECT1_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SQUELCH_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern void bell(cptr fmt, ...);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MAIN_CRB_C) || defined(MAIN_MAC_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(UTIL_C) || defined(XTRA2_C))
extern void sound(int val);
#endif
#if (defined(MAIN_CRB_C) || defined(UTIL_C) || defined(XTRA1_C))
extern bool screen_is_icky(void);
#endif
#if (defined(SPELLS2_C) || defined(SQUELCH_C) || defined(UTIL_C) || defined(WIZARD2_C))
extern void set_gnext(cptr next);
#endif
#if (defined(SQUELCH_C) || defined(UTIL_C))
extern bool gnext_clear(void);
#endif
#if (defined(FILES_C) || defined(UTIL_C))
extern bool is_keymap_or_macro(void);
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_DOS_C) || defined(MAIN_ROS_C) || defined(OBJECT1_C) || defined(SPELLS2_C) || defined(UTIL_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern char inkey(void);
#endif
#if (defined(CMD3_C) || defined(FILES_C) || defined(LOAD_C) || defined(OBJECT2_C) || defined(SPELLS2_C) || defined(SQUELCH_C) || defined(UTIL_C))
extern s16b quark_add(cptr str);
#endif
#if (defined(BIRTH_C) || defined(CMD2_C) || defined(CMD3_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SAVE_C) || defined(SQUELCH_C) || defined(UTIL_C))
extern cptr quark_str(u16b i);
#endif
#if (defined(CMD4_C) || defined(SAVE_C) || defined(UTIL_C) || defined(XTRA1_C))
extern s16b message_num(void);
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(SAVE_C) || defined(UTIL_C) || defined(XTRA1_C))
extern cptr message_str(s16b age);
#endif
#if (defined(BIRTH_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(LOAD_C) || defined(STORE_C) || defined(UTIL_C))
extern void message_add(cptr str);
#endif
#if (defined(DUNGEON_C) || defined(SPELLS1_C) || defined(UTIL_C) || defined(XTRA1_C))
extern bool no_msg_print;
#endif
#if (defined(ANGBAND_H))
extern void msg_print(cptr msg);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(GENERATE_C) || defined(INIT1_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER1_C) || defined(MONSTER2_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(QUEST_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void msg_format(cptr fmt, ...);
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(UTIL_C) || defined(XTRA1_C))
extern void c_put_str(byte attr, cptr str, int row, int col);
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(SQUELCH_C) || defined(STORE_C) || defined(UTIL_C))
extern void put_str(cptr str, int row, int col);
#endif
#if (defined(CMD4_C) || defined(UTIL_C) || defined(XTRA1_C))
extern void c_prt(byte attr, cptr str, int row, int col);
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(CMD1_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(LOAD_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(OBJECT1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void prt(cptr str, int row, int col);
#endif
#if (defined(CMD5_C) || defined(UTIL_C))
extern void mc_roff_xy(int x, int y, cptr s);
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(SQUELCH_C) || defined(STORE_C) || defined(UTIL_C))
extern void mc_roff(cptr s);
#endif
#if (defined(MONSTER1_C) || defined(UTIL_C))
extern bool c_roff(byte a, cptr str);
#endif
#if (defined(CMD4_C) || defined(MONSTER1_C) || defined(UTIL_C))
extern void roff(cptr str);
#endif
#if (defined(CMD4_C) || defined(UTIL_C) || defined(XTRA1_C))
extern void mc_put_str(const int y, const int x, cptr str);
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(CMD5_C) || defined(FILES_C) || defined(MONSTER1_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SQUELCH_C) || defined(UTIL_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void mc_put_fmt(const int y, const int x, cptr fmt, ...);
#endif
#if (defined(UTIL_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void mc_put_lfmt(const int y, const int x, const int l, cptr fmt, ...);
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(SQUELCH_C) || defined(STORE_C) || defined(UTIL_C) || defined(XTRA1_C))
extern void clear_from(int row);
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(SQUELCH_C) || defined(UTIL_C) || defined(WIZARD2_C))
extern bool askfor_aux(char *buf, int len);
#endif
#if (defined(BIRTH_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(SQUELCH_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD2_C))
extern bool get_string(cptr prompt, char *buf, int len);
#endif
#if (defined(CMD2_C) || defined(STORE_C) || defined(UTIL_C))
extern char get_check_aux(cptr prompt, cptr text, cptr conv_from, cptr conv_to);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(UTIL_C))
extern bool get_check(cptr prompt);
#endif
#if (defined(CMD1_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(SPELLS2_C) || defined(SQUELCH_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern bool get_com(char *command, cptr fmt, ...);
#endif
#if (defined(CMD3_C) || defined(STORE_C) || defined(UTIL_C))
extern s16b get_quantity(cptr prompt, int max,bool allbydefault);
#endif
#if (defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_CRB_C) || defined(MAIN_EMX_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(UTIL_C))
extern void pause_line(void);
#endif
#if (defined(CMD4_C) || defined(UTIL_C))
extern int keymap_mode(void);
#endif
#if (defined(DUNGEON_C) || defined(SQUELCH_C) || defined(STORE_C) || defined(UTIL_C))
extern void request_command(bool shopping);
#endif
#if (defined(OBJECT1_C) || defined(POWERS_C) || defined(UTIL_C))
extern bool is_a_vowel(int ch);
#endif
#if (defined(CMD3_C) || defined(CMD4_C) || defined(UTIL_C) || defined(XTRA2_C))
extern int get_keymap_dir(char ch);
#endif
#if (defined(ALLOW_REPEAT)) && (defined(CMD5_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(UTIL_C) || defined(XTRA2_C))
extern  void repeat_push(int what);
#endif
#if (defined(ALLOW_REPEAT)) && (defined(CMD5_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(UTIL_C) || defined(XTRA2_C))
extern  bool repeat_pull(int *what);
#endif
#if (defined(ALLOW_REPEAT)) && (defined(DUNGEON_C) || defined(STORE_C) || defined(UTIL_C))
extern  void repeat_check(void);
#endif
#if (defined(SUPPORT_GAMMA)) && (defined(MAID_X11_C) || defined(MAIN_GTK_C) || defined(MAIN_WIN_C) || defined(UTIL_C))
extern byte gamma_table[256];
#endif
#if (defined(SUPPORT_GAMMA)) && (defined(MAID_X11_C) || defined(MAIN_GTK_C) || defined(MAIN_WIN_C) || defined(UTIL_C))
extern void build_gamma_table(int gamma);
#endif
#if (defined(CMD4_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(UTIL_C))
extern errr add_resize_hook(void (*resize_hook)(void));
#endif
#if (defined(CMD4_C) || defined(CMD5_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(UTIL_C))
extern errr delete_resize_hook(void (*resize_hook)(void));
#endif
#if (defined(DUNGEON_C) || defined(UTIL_C))
extern void resize_main_term(void);
#endif
#if (defined(CMD4_C) || defined(STORE_C) || defined(UTIL_C))
extern void repeat_string_f2(char *buf, uint max, cptr UNUSED fmt, va_list *vp);
#endif
#if (defined(MAIN_X11_C) || defined(UTIL_C))
extern errr type_string(cptr str, uint len);
#endif

/* variable.c */

#if (defined(VARIABLE_C))
extern cptr copyright[5];
#endif
#if (defined(FILES_C) || defined(INIT2_C) || defined(VARIABLE_C))
extern cptr syshelpfile;
#endif
#if (defined(LOAD_C) || defined(MAIN_CRB_C) || defined(MAIN_EMX_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern bool arg_fiddle;
#endif
#if (defined(MAIN_CRB_C) || defined(MAIN_EMX_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(VARIABLE_C))
extern bool arg_wizard;
#endif
#if (defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern bool arg_sound;
#endif
#if (defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_GCU_C) || defined(MAIN_GTK_C) || defined(MAIN_IBM_C) || defined(MAIN_LSL_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern bool arg_graphics;
#endif
#if (defined(DUNGEON_C) || defined(MAIN_EMX_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern bool arg_force_original;
#endif
#if (defined(DUNGEON_C) || defined(MAIN_EMX_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern bool arg_force_roguelike;
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_CRB_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(UTIL_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern bool character_generated;
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(MONSTER2_C) || defined(OBJECT2_C) || defined(VARIABLE_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern bool character_dungeon;
#endif
#if (defined(DUNGEON_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern bool character_loaded;
#endif
#if (defined(FILES_C) || defined(SAVE_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool character_saved;
#endif
#if (defined(CAVE_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(STORE_C) || defined(UTIL_C) || defined(VARIABLE_C) || defined(WIZARD1_C) || defined(WIZARD2_C))
extern bool character_icky;
#endif
#if (defined(DUNGEON_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern bool character_xtra;
#endif
#if (defined(DUNGEON_C) || defined(LOAD_C) || defined(OBJECT1_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern u32b seed_flavor;
#endif
#if (defined(DUNGEON_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern u32b seed_wild;
#endif
#if (defined(DUNGEON_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern s16b command_cmd;
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(DUNGEON_C) || defined(STORE_C) || defined(UTIL_C) || defined(VARIABLE_C) || defined(WIZARD2_C))
extern s16b command_arg;
#endif
#if (defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(DUNGEON_C) || defined(SQUELCH_C) || defined(STORE_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern s16b command_rep;
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C) || defined(UTIL_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern s16b command_dir;
#endif
#if (defined(CMD3_C) || defined(DUNGEON_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(VARIABLE_C))
extern s16b command_see;
#endif
#if (defined(CMD3_C) || defined(OBJECT1_C) || defined(VARIABLE_C))
extern s16b command_wrk;
#endif
#if (defined(CMD3_C) || defined(OBJECT1_C) || defined(VARIABLE_C))
extern s16b command_gap;
#endif
#if (defined(CMD2_C) || defined(CMD3_C) || defined(DUNGEON_C) || defined(STORE_C) || defined(UTIL_C) || defined(VARIABLE_C) || defined(WIZARD2_C))
extern s16b command_new;
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(OBJECT1_C) || defined(SPELLS2_C) || defined(SQUELCH_C) || defined(STORE_C) || defined(VARIABLE_C))
extern s16b energy_use;
#endif
#if (defined(DUNGEON_C) || defined(SQUELCH_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern s16b old_energy_use;
#endif
#if (defined(DUNGEON_C) || defined(VARIABLE_C))
extern bool create_up_stair;
#endif
#if (defined(DUNGEON_C) || defined(VARIABLE_C))
extern bool create_down_stair;
#endif
#if (defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_CRB_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool msg_flag;
#endif
#if (defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_ROS_C) || defined(MELEE2_C) || defined(SQUELCH_C) || defined(VARIABLE_C))
extern bool alive;
#endif
#if (defined(DUNGEON_C) || defined(FILES_C) || defined(LOAD_C) || defined(MAIN_ROS_C) || defined(MELEE2_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(SQUELCH_C) || defined(VARIABLE_C))
extern bool death;
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(DUNGEON_C) || defined(SQUELCH_C) || defined(VARIABLE_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern s16b running;
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(DUNGEON_C) || defined(SQUELCH_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern s16b resting;
#endif
#if (defined(ANGBAND_H))
extern s16b cur_hgt;
#endif
#if (defined(ANGBAND_H))
extern s16b cur_wid;
#endif
#if (defined(ANGBAND_H))
extern s16b dun_level;
#endif
#if (defined(ANGBAND_H))
extern s16b dun_offset;
#endif
#if (defined(DUNGEON_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(MONSTER2_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern u16b dun_bias;
#endif
#if (defined(CMD2_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(SAVE_C) || defined(STORE_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern byte cur_town;
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(OBJECT2_C) || defined(QUEST_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(VARIABLE_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern byte cur_dungeon;
#endif
#if (defined(CMD2_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(LOAD_C) || defined(SAVE_C) || defined(SPELLS2_C) || defined(VARIABLE_C))
extern byte recall_dungeon;
#endif
#if (defined(DUNGEON_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(SAVE_C) || defined(STORE_C) || defined(VARIABLE_C))
extern byte came_from;
#endif
#if (defined(LOAD_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern s16b num_repro;
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(OBJECT2_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern s16b object_level;
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(SAVE_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern s32b turn;
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern s32b old_turn;
#endif
#if (defined(DUNGEON_C) || defined(LOAD_C) || defined(SAVE_C) || defined(SPELLS2_C) || defined(VARIABLE_C))
extern s32b curse_turn;
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(LOAD_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SAVE_C) || defined(SPELLS2_C) || defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern bool cheat_wzrd;
#endif
#if (defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool use_sound;
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_GCU_C) || defined(MAIN_GTK_C) || defined(MAIN_IBM_C) || defined(MAIN_LSL_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(OBJECT1_C) || defined(SPELLS1_C) || defined(TABLES_C) || defined(VARIABLE_C) || defined(Z_TERM_C))
extern bool use_graphics;
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(LOAD_C) || defined(MAIN_AMI_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern u16b total_winner;
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern u16b panic_save;
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern u16b noscore;
#endif
#if (defined(MAIN_DOS_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(VARIABLE_C))
extern bool use_transparency;
#endif
#if (defined(FILES_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern s16b signal_count;
#endif
#if (defined(CMD4_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool inkey_base;
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(UTIL_C) || defined(VARIABLE_C) || defined(WIZARD2_C))
extern bool inkey_scan;
#endif
#if (defined(MAIN_CRB_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool inkey_flag;
#endif
#if (defined(OBJECT2_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern s16b coin_type;
#endif
#if (defined(CMD2_C) || defined(OBJECT2_C) || defined(VARIABLE_C))
extern bool opening_chest;
#endif
#if (defined(DUNGEON_C) || defined(MONSTER2_C) || defined(VARIABLE_C))
extern bool shimmer_monsters;
#endif
#if (defined(DUNGEON_C) || defined(VARIABLE_C))
extern bool shimmer_objects;
#endif
#if (defined(DUNGEON_C) || defined(MONSTER2_C) || defined(SPELLS2_C) || defined(VARIABLE_C))
extern bool repair_monsters;
#endif
#if (defined(DUNGEON_C) || defined(VARIABLE_C))
extern bool repair_objects;
#endif
#if (defined(BIRTH_C) || defined(CMD3_C) || defined(CMD6_C) || defined(LOAD_C) || defined(OBJECT2_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern s16b total_weight;
#endif
#if (defined(DUNGEON_C) || defined(MONSTER2_C) || defined(VARIABLE_C))
extern bool hack_mind;
#endif
#if (defined(BIRTH_C) || defined(DUNGEON_C) || defined(VARIABLE_C))
extern bool hack_chaos_feature;
#endif
#if (defined(OBJECT2_C) || defined(SPELLS2_C) || defined(VARIABLE_C))
extern int artifact_bias;
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(INIT1_C) || defined(OBJECT2_C) || defined(SAVE_C) || defined(SPELLS2_C) || defined(VARIABLE_C))
extern s16b o_max;
#endif
#if (defined(DUNGEON_C) || defined(OBJECT2_C) || defined(VARIABLE_C))
extern s16b o_cnt;
#endif
#if (defined(CMD1_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(INIT1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(SAVE_C) || defined(SPELLS2_C) || defined(VARIABLE_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern s16b m_max;
#endif
#if (defined(DUNGEON_C) || defined(MONSTER2_C) || defined(VARIABLE_C))
extern s16b m_cnt;
#endif
#if (defined(MELEE2_C) || defined(MONSTER2_C) || defined(VARIABLE_C))
extern s16b hack_m_idx;
#endif
#if (defined(MONSTER2_C) || defined(VARIABLE_C))
extern s16b hack_m_idx_ii;
#endif
#if (defined(DUNGEON_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern bool multi_rew;
#endif
#if (defined(DUNGEON_C) || defined(MELEE2_C) || defined(VARIABLE_C))
extern int total_friends;
#endif
#if (defined(DUNGEON_C) || defined(MELEE2_C) || defined(VARIABLE_C))
extern s32b total_friend_levels;
#endif
#if (defined(OBJECT2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool inscribe_depth;
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(STORE_C) || defined(TABLES_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool rogue_like_commands;
#endif
#if (defined(TABLES_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool quick_messages;
#endif
#if (defined(TABLES_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool quick_prompt;
#endif
#if (defined(OBJECT1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool other_query_flag;
#endif
#if (defined(CMD2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool carry_query_flag;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern bool use_old_target;
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool always_pickup;
#endif
#if (defined(TABLES_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool always_repeat;
#endif
#if (defined(MONSTER1_C) || defined(OBJECT2_C) || defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern bool depth_in_feet;
#endif
#if (defined(OBJECT2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool stack_force_notes;
#endif
#if (defined(OBJECT2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool stack_force_notes_all;
#endif
#if (defined(OBJECT2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool stack_force_costs;
#endif
#if (defined(OBJECT1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool show_labels;
#endif
#if (defined(OBJECT1_C) || defined(STORE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool show_weights;
#endif
#if (defined(CMD5_C) || defined(OBJECT1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool show_choices;
#endif
#if (defined(MONSTER1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool show_details;
#endif
#if (defined(CMD1_C) || defined(CMD5_C) || defined(OBJECT1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool show_choices_main;
#endif
#if (defined(TABLES_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool ring_bell;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C) || defined(Z_TERM_C))
extern bool use_color;
#endif
#if (defined(STORE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool verbose_haggle;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern bool scroll_edge;
#endif
#if (defined(CAVE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool show_piles;
#endif
#if (defined(DUNGEON_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool beginner_help;
#endif
#if (defined(CAVE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool allow_fake_colour;
#endif
#if (defined(CMD2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool find_ignore_stairs;
#endif
#if (defined(CMD2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool find_ignore_doors;
#endif
#if (defined(CMD2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool find_cut;
#endif
#if (defined(CMD2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool stop_corner;
#endif
#if (defined(CMD2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool find_examine;
#endif
#if (defined(MELEE2_C) || defined(MONSTER2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool disturb_move;
#endif
#if (defined(MELEE2_C) || defined(MONSTER2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool disturb_near;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern bool disturb_panel;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern bool disturb_state;
#endif
#if (defined(DUNGEON_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool disturb_dawn;
#endif
#if (defined(DUNGEON_C) || defined(MELEE2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool disturb_minor;
#endif
#if (defined(CMD2_C) || defined(MONSTER1_C) || defined(MONSTER2_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(TABLES_C) || defined(UTIL_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern bool alert_failure;
#endif
#if (defined(SPELLS1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool last_words;
#endif
#if (defined(FILES_C) || defined(GENERATE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool small_levels;
#endif
#if (defined(FILES_C) || defined(GENERATE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool empty_levels;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C))
extern bool player_symbols;
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool equippy_chars;
#endif
#if (defined(FILES_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool skip_chaos_features;
#endif
#if (defined(OBJECT1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool plain_descriptions;
#endif
#if (defined(FILES_C) || defined(MELEE2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool stupid_monsters;
#endif
#if (defined(CMD3_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool auto_destroy;
#endif
#if (defined(CMD2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool confirm_stairs;
#endif
#if (defined(CMD3_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool wear_confirm;
#endif
#if (defined(CMD3_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool confirm_wear_all;
#endif
#if (defined(MELEE2_C) || defined(MONSTER2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool disturb_allies;
#endif
#if (defined(CMD2_C) || defined(FILES_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool multi_stair;
#endif
#if (defined(OBJECT1_C) || defined(STORE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool unify_commands;
#endif
#if (defined(CMD3_C) || defined(DUNGEON_C) || defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern bool centre_view;
#endif
#if (defined(TABLES_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool macro_edit;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern bool no_centre_run;
#endif
#if (defined(CAVE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool track_mouse;
#endif
#if (defined(TABLES_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool auto_more;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C))
extern bool preserve_mode_w;
#endif
#if (defined(CMD3_C) || defined(FILES_C) || defined(GENERATE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool preserve_mode;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C))
extern bool maximise_mode_w;
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern bool maximise_mode;
#endif
#if (defined(BIRTH_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool use_autoroller;
#endif
#if (defined(BIRTH_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool spend_points;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C))
extern bool ironman_shop_w;
#endif
#if (defined(FILES_C) || defined(STORE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool ironman_shop;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C))
extern bool ironman_feeling_w;
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool ironman_feeling;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C))
extern bool speak_unique_w;
#endif
#if (defined(MELEE2_C) || defined(STORE_C) || defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern bool speak_unique;
#endif
#if (defined(SCORE_QUITTERS)) && (defined(TABLES_C) || defined(VARIABLE_C))
extern bool score_quitters_w;
#endif
#if (defined(SCORE_QUITTERS)) && (defined(FILES_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool score_quitters;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C))
extern bool chaos_patrons_w;
#endif
#if (defined(DUNGEON_C) || defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern bool chaos_patrons;
#endif
#if (defined(OBJECT1_C) || defined(STORE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool auto_haggle;
#endif
#if (defined(FILES_C) || defined(GENERATE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool auto_scum;
#endif
#if (defined(OBJECT2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool stack_allow_items;
#endif
#if (defined(OBJECT2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool stack_allow_wands;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern bool expand_look;
#endif
#if (defined(CMD3_C) || defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern bool expand_list;
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool view_perma_grids;
#endif
#if (defined(CAVE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool view_torch_grids;
#endif
#if (defined(GENERATE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool dungeon_align;
#endif
#if (defined(DUNGEON_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool dungeon_stair;
#endif
#if (defined(FILES_C) || defined(GENERATE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool dungeon_small;
#endif
#if (defined(CAVE_C) || defined(MELEE2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool flow_by_sound;
#endif
#if (defined(MELEE2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool flow_by_smell;
#endif
#if (defined(MELEE2_C) || defined(MONSTER2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool smart_learn;
#endif
#if (defined(MELEE2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool smart_cheat;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern bool view_reduce_lite;
#endif
#if (defined(CAVE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool view_reduce_view;
#endif
#if (defined(DUNGEON_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool avoid_abort;
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool avoid_other;
#endif
#if (defined(TABLES_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool flush_error;
#endif
#if (defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool flush_failure;
#endif
#if (defined(CAVE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool flush_disturb;
#endif
#if (defined(TABLES_C) || defined(VARIABLE_C))
extern bool flush_command;
#endif
#if (defined(DUNGEON_C) || defined(SQUELCH_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool fresh_before;
#endif
#if (defined(DUNGEON_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool fresh_after;
#endif
#if (defined(TABLES_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool fresh_message;
#endif
#if (defined(SAVE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool compress_savefile;
#endif
#if (defined(TABLES_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern bool hilite_player;
#endif
#if (defined(CAVE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool view_yellow_lite;
#endif
#if (defined(CAVE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool view_bright_lite;
#endif
#if (defined(CAVE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool view_granite_lite;
#endif
#if (defined(CAVE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool view_special_lite;
#endif
#if (defined(OBJECT2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool testing_stack;
#endif
#if (defined(MELEE1_C) || defined(MELEE2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool testing_carry;
#endif
#if (defined(OBJECT1_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool spoil_art;
#endif
#if (defined(CMD3_C) || defined(CMD4_C) || defined(MONSTER1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool spoil_mon;
#endif
#if (defined(OBJECT1_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool spoil_ego;
#endif
#if (defined(OBJECT1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool spoil_value;
#endif
#if (defined(CMD4_C) || defined(OBJECT1_C) || defined(SQUELCH_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool spoil_base;
#endif
#if (defined(OBJECT1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool spoil_stat;
#endif
#if (defined(OBJECT1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool spoil_dam;
#endif
#if (defined(MONSTER1_C) || defined(OBJECT1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool spoil_flag;
#endif
#if (defined(CMD5_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(OBJECT2_C) || defined(SAVE_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool cheat_peek;
#endif
#if (defined(GENERATE_C) || defined(LOAD_C) || defined(MONSTER2_C) || defined(SAVE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool cheat_hear;
#endif
#if (defined(GENERATE_C) || defined(LOAD_C) || defined(SAVE_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool cheat_room;
#endif
#if (defined(CMD1_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(MONSTER2_C) || defined(SAVE_C) || defined(SPELLS2_C) || defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern bool cheat_xtra;
#endif
#if (defined(BIRTH_C) || defined(OBJECT1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool cheat_item;
#endif
#if (defined(DUNGEON_C) || defined(LOAD_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool cheat_live;
#endif
#if (defined(CMD4_C) || defined(LOAD_C) || defined(SAVE_C) || defined(TABLES_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern bool cheat_skll;
#endif
#if (defined(FILES_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool cheat_save;
#endif
#if (defined(BIRTH_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool allow_quickstart;
#endif
#if (defined(USE_MAIN_C)) && (defined(DUNGEON_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool display_credits;
#endif
#if (defined(BIRTH_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern bool allow_pickstats;
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(LOAD_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(VARIABLE_C))
extern s16b hitpoint_warn;
#endif
#if (defined(CMD1_C) || defined(CMD4_C) || defined(LOAD_C) || defined(POWERS_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(VARIABLE_C))
extern s16b delay_factor;
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern bool autosave_l;
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern bool autosave_t;
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern bool autosave_q;
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern s16b autosave_freq;
#endif
#if (defined(CMD4_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern s16b feeling;
#endif
#if (defined(GENERATE_C) || defined(MONSTER2_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(VARIABLE_C))
extern s16b rating;
#endif
#if (defined(GENERATE_C) || defined(OBJECT2_C) || defined(VARIABLE_C))
extern bool good_item_flag;
#endif
#if (defined(DUNGEON_C) || defined(MELEE2_C) || defined(SQUELCH_C) || defined(STORE_C) || defined(VARIABLE_C))
extern bool new_level_flag;
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(VARIABLE_C))
extern int full_grid;
#endif
#if (defined(CMD3_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern s16b max_panel_rows;
#endif
#if (defined(CMD3_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern s16b max_panel_cols;
#endif
#if (defined(CMD3_C) || defined(GENERATE_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern s16b panel_row;
#endif
#if (defined(CMD3_C) || defined(GENERATE_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern s16b panel_col;
#endif
#if (defined(ANGBAND_H))
extern s16b panel_row_min;
#endif
#if (defined(ANGBAND_H))
extern s16b panel_row_max;
#endif
#if (defined(ANGBAND_H))
extern s16b panel_col_min;
#endif
#if (defined(ANGBAND_H))
extern s16b panel_col_max;
#endif
#if (defined(CAVE_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern s16b panel_col_prt;
#endif
#if (defined(CAVE_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern s16b panel_row_prt;
#endif
#if (defined(ANGBAND_H))
extern s16b py;
#endif
#if (defined(ANGBAND_H))
extern s16b px;
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(SAVE_C) || defined(SPELLS2_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern s16b wildx;
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(SAVE_C) || defined(SPELLS2_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern s16b wildy;
#endif
#if (defined(DUNGEON_C) || defined(MONSTER2_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern s16b target_who;
#endif






#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(VARIABLE_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern s16b health_who;
#endif
#if (defined(CAVE_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern s16b monster_race_idx;
#endif
#if (defined(CAVE_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern s16b object_kind_idx;
#endif
#if (defined(CAVE_C) || defined(OBJECT1_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern object_type *tracked_o_ptr;
#endif
#if (defined(CAVE_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern co_ord tracked_co_ord;
#endif
#if (defined(FILES_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern int player_uid;
#endif
#if (defined(FILES_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern int player_euid;
#endif
#if (defined(FILES_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern int player_egid;
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(LOAD_C) || defined(MAIN_EMX_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern char player_name[NAME_LEN];
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(SQUELCH_C) || defined(VARIABLE_C))
extern char player_base[NAME_LEN];
#endif
#if (defined(DUNGEON_C) || defined(FILES_C) || defined(LOAD_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(VARIABLE_C))
extern cptr died_from;
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern char history[4][60];
#endif
#if (defined(FILES_C) || defined(LOAD_C) || defined(MAIN_CRB_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern char savefile[1024];
#endif
#if (defined(CAVE_C) || defined(VARIABLE_C))
extern s16b lite_n;
#endif
#if (defined(CAVE_C) || defined(VARIABLE_C))
extern byte lite_y[LITE_MAX];
#endif
#if (defined(CAVE_C) || defined(VARIABLE_C))
extern byte lite_x[LITE_MAX];
#endif
#if (defined(CAVE_C) || defined(VARIABLE_C))
extern s16b view_n;
#endif
#if (defined(CAVE_C) || defined(VARIABLE_C))
extern byte view_y[VIEW_MAX];
#endif
#if (defined(CAVE_C) || defined(VARIABLE_C))
extern byte view_x[VIEW_MAX];
#endif
#if (defined(CAVE_C) || defined(SPELLS2_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern s16b temp_n;
#endif
#if (defined(CAVE_C) || defined(SPELLS2_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern byte temp_y[TEMP_MAX];
#endif
#if (defined(CAVE_C) || defined(SPELLS2_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern byte temp_x[TEMP_MAX];
#endif
#if (defined(CMD4_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern s16b macro__num;
#endif
#if (defined(CMD4_C) || defined(INIT2_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern cptr *macro__pat;
#endif
#if (defined(CMD4_C) || defined(INIT2_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern cptr *macro__act;
#endif
#if (defined(INIT2_C) || defined(VARIABLE_C))
extern bool *macro__cmd;
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(INIT2_C) || defined(VARIABLE_C))
extern char *macro__buf;
#endif
#if (defined(UTIL_C) || defined(VARIABLE_C))
extern s16b quark__num;
#endif
#if (defined(INIT2_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern cptr *quark__str;
#endif
#if (defined(UTIL_C) || defined(VARIABLE_C))
extern u16b message__next;
#endif
#if (defined(UTIL_C) || defined(VARIABLE_C))
extern u16b message__last;
#endif
#if (defined(UTIL_C) || defined(VARIABLE_C))
extern u16b message__head;
#endif
#if (defined(INIT2_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern u16b message__tail;
#endif
#if (defined(INIT2_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern u16b *message__ptr;
#endif
#if (defined(INIT2_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern char *message__buf;
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_GCU_C) || defined(MAIN_GTK_C) || defined(MAIN_IBM_C) || defined(MAIN_LSL_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_VCS_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(VARIABLE_C))
extern byte angband_color_table[256][4];
#endif
#if (defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(VARIABLE_C))
extern char angband_sound_name[SOUND_MAX][16];
#endif
#if (defined(ANGBAND_H))
extern cave_type *cave[MAX_HGT];
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(SQUELCH_C) || defined(VARIABLE_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern object_type *o_list;
#endif
#if (defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD4_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(VARIABLE_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern monster_type *m_list;
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(GENERATE_C) || defined(INIT2_C) || defined(LOAD_C) || defined(SAVE_C) || defined(STORE_C) || defined(VARIABLE_C))
extern store_type *store;
#endif
#if (defined(BIRTH_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(SQUELCH_C) || defined(STORE_C) || defined(UTIL_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern object_type *inventory;
#endif
#if (defined(INIT2_C) || defined(OBJECT2_C) || defined(VARIABLE_C))
extern s16b alloc_kind_size;
#endif
#if (defined(INIT2_C) || defined(OBJECT2_C) || defined(VARIABLE_C))
extern alloc_entry *alloc_kind_table;
#endif
#if (defined(INIT2_C) || defined(MONSTER2_C) || defined(VARIABLE_C))
extern s16b alloc_race_size;
#endif
#if (defined(INIT2_C) || defined(MONSTER2_C) || defined(VARIABLE_C))
extern alloc_entry *alloc_race_table;
#endif
#if (defined(FILES_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(VARIABLE_C))
extern byte tval_to_attr[128];
#endif
#if (defined(FILES_C) || defined(OBJECT1_C) || defined(VARIABLE_C))
extern char tval_to_char[128];
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(UTIL_C) || defined(VARIABLE_C))
extern cptr keymap_act[KEYMAP_MODES][256];
#endif
#if (defined(OBJECT1_C) || defined(VARIABLE_C))
extern player_type p_body;
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(LOAD_C) || defined(MAIN_AMI_C) || defined(MAIN_WIN_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(SQUELCH_C) || defined(STORE_C) || defined(UTIL_C) || defined(VARIABLE_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern player_type *p_ptr;
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(LOAD_C) || defined(VARIABLE_C))
extern player_sex *sp_ptr;
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(LOAD_C) || defined(POWERS_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern player_race *rp_ptr;
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(LOAD_C) || defined(MAIN_AMI_C) || defined(SPELLS2_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern player_template *cp_ptr;
#endif
#if (defined(BIRTH_C) || defined(CMD5_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern byte spell_order[128];
#endif
#if (defined(BIRTH_C) || defined(LOAD_C) || defined(SAVE_C) || defined(VARIABLE_C) || defined(WIZARD2_C) || defined(XTRA1_C))
extern s16b player_hp[100];
#endif
#if (defined(ANGBAND_H))
extern maxima *z_info;
#endif
#if (defined(GENERATE_C) || defined(INIT2_C) || defined(VARIABLE_C))
extern vault_type *v_info;
#endif
#if (defined(GENERATE_C) || defined(INIT2_C) || defined(VARIABLE_C))
extern cptr v_name;
#endif
#if (defined(GENERATE_C) || defined(INIT2_C) || defined(VARIABLE_C))
extern cptr v_text;
#endif
#if (defined(CAVE_C) || defined(CMD4_C) || defined(FILES_C) || defined(INIT2_C) || defined(MAIN_AMI_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern feature_type *f_info;
#endif
#if (defined(INIT2_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern cptr f_name;
#endif
#if (defined(ANGBAND_H))
extern object_kind *k_info;
#endif
#if (defined(INIT1_C) || defined(INIT2_C) || defined(OBJECT1_C) || defined(VARIABLE_C))
extern cptr k_name;
#endif
#if (defined(INIT2_C) || defined(OBJECT1_C) || defined(VARIABLE_C))
extern cptr k_text;
#endif
#if (defined(ANGBAND_H))
extern unident_type *u_info;
#endif
#if (defined(INIT2_C) || defined(OBJECT1_C) || defined(VARIABLE_C))
extern char *u_name;
#endif
#if (defined(INIT1_C) || defined(INIT2_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(VARIABLE_C))
extern o_base_type *o_base;
#endif
#if (defined(INIT1_C) || defined(INIT2_C) || defined(OBJECT1_C) || defined(VARIABLE_C))
extern cptr ob_name;
#endif
#if (defined(BIRTH_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD6_C) || defined(INIT1_C) || defined(INIT2_C) || defined(LOAD_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SAVE_C) || defined(VARIABLE_C) || defined(WIZARD1_C) || defined(WIZARD2_C))
extern artifact_type *a_info;
#endif
#if (defined(INIT1_C) || defined(INIT2_C) || defined(OBJECT1_C) || defined(VARIABLE_C))
extern cptr a_name;
#endif
#if (defined(INIT1_C) || defined(INIT2_C) || defined(LOAD_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(VARIABLE_C))
extern ego_item_type *e_info;
#endif
#if (defined(INIT1_C) || defined(INIT2_C) || defined(OBJECT1_C) || defined(VARIABLE_C))
extern cptr e_name;
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(GENERATE_C) || defined(INIT1_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER1_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(QUEST_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(VARIABLE_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern monster_race *r_info;
#endif
#if (defined(INIT2_C) || defined(LOAD_C) || defined(MONSTER2_C) || defined(SAVE_C) || defined(VARIABLE_C))
extern char *r_name;
#endif
#if (defined(INIT2_C) || defined(MONSTER1_C) || defined(VARIABLE_C) || defined(WIZARD1_C))
extern cptr r_text;
#endif
#if (defined(INIT2_C) || defined(LOAD_C) || defined(MONSTER1_C) || defined(SAVE_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern death_event_type *death_event;
#endif
#if (defined(INIT2_C) || defined(OBJECT2_C) || defined(VARIABLE_C))
extern cptr event_name;
#endif
#if (defined(INIT2_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern cptr event_text;
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(CMD2_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(GENERATE_C) || defined(INIT1_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MONSTER1_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(VARIABLE_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern dun_type *dun_defs;
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(CMD4_C) || defined(FILES_C) || defined(INIT1_C) || defined(INIT2_C) || defined(VARIABLE_C) || defined(XTRA1_C))
extern cptr dun_name;
#endif
#if (defined(ANGBAND_H))
extern town_type *town_defs;
#endif
#if (defined(CAVE_C) || defined(CMD2_C) || defined(CMD4_C) || defined(INIT2_C) || defined(STORE_C) || defined(VARIABLE_C))
extern cptr town_name;
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MONSTER1_C) || defined(QUEST_C) || defined(SAVE_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern quest_type *q_list;
#endif
#if (defined(INIT2_C) || defined(STORE_C) || defined(VARIABLE_C))
extern owner_type *owners;
#endif
#if (defined(INIT2_C) || defined(STORE_C) || defined(VARIABLE_C))
extern cptr s_name;
#endif
#if (defined(CAVE_C) || defined(CMD4_C) || defined(FILES_C) || defined(INIT2_C) || defined(MAIN_CRB_C) || defined(MAIN_EMX_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(MAIN_XXX_C) || defined(MAIN_C) || defined(OBJECT1_C) || defined(VARIABLE_C))
extern cptr ANGBAND_SYS;
#endif
#if (defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_MAC_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(VARIABLE_C))
extern cptr ANGBAND_GRAF;
#endif
#if (defined(INIT2_C) || defined(VARIABLE_C))
extern cptr ANGBAND_DIR;
#endif
#if (defined(FILES_C) || defined(INIT2_C) || defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern cptr ANGBAND_DIR_APEX;
#endif
#if (defined(FILES_C) || defined(INIT2_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(MONSTER2_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern cptr ANGBAND_DIR_BONE;
#endif
#if (defined(INIT2_C) || defined(MAIN_CRB_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern cptr ANGBAND_DIR_DATA;
#endif
#if (defined(INIT2_C) || defined(MAIN_CRB_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern cptr ANGBAND_DIR_EDIT;
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(MAIN_CRB_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern cptr ANGBAND_DIR_FILE;
#endif
#if (defined(FILES_C) || defined(INIT2_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern cptr ANGBAND_DIR_HELP;
#endif
#if (defined(FILES_C) || defined(INIT2_C) || defined(MAIN_EMX_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern cptr ANGBAND_DIR_INFO;
#endif
#if (defined(FILES_C) || defined(INIT2_C) || defined(MAIN_WIN_C) || defined(VARIABLE_C))
extern cptr ANGBAND_DIR_PREF;
#endif
#if (defined(FILES_C) || defined(INIT2_C) || defined(MAIN_CRB_C) || defined(MAIN_EMX_C) || defined(MAIN_GTK_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern cptr ANGBAND_DIR_SAVE;
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(INIT2_C) || defined(MAIN_AMI_C) || defined(MAIN_DOS_C) || defined(MAIN_EMX_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(SQUELCH_C) || defined(VARIABLE_C) || defined(WIZARD1_C))
extern cptr ANGBAND_DIR_USER;
#endif
#if (defined(INIT2_C) || defined(MAIN_AMI_C) || defined(MAIN_DOS_C) || defined(MAIN_GTK_C) || defined(MAIN_IBM_C) || defined(MAIN_LSL_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(MAIN_C) || defined(VARIABLE_C))
extern cptr ANGBAND_DIR_XTRA;
#endif






#if (defined(CMD3_C) || defined(OBJECT1_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(VARIABLE_C))
extern bool (*item_tester_hook)(object_ctype*);
#endif
#if (defined(CMD3_C) || defined(CMD4_C) || defined(OBJECT2_C) || defined(SQUELCH_C) || defined(VARIABLE_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern bool (*ang_sort_comp)(vptr u, vptr v, int a, int b);
#endif
#if (defined(CMD3_C) || defined(CMD4_C) || defined(OBJECT2_C) || defined(SQUELCH_C) || defined(VARIABLE_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void (*ang_sort_swap)(vptr u, vptr v, int a, int b);
#endif
#if (defined(CAVE_C) || defined(VARIABLE_C) || defined(XTRA2_C))
extern bool violet_uniques;
#endif
#if (defined(ALLOW_EASY_OPEN)) && (defined(CMD2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern  bool easy_open;
#endif
#if (defined(ALLOW_EASY_DISARM)) && (defined(CMD2_C) || defined(TABLES_C) || defined(VARIABLE_C))
extern  bool easy_disarm;
#endif
#if (defined(BIRTH_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(VARIABLE_C))
extern stat_default_type *stat_default;
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(VARIABLE_C))
extern s16b stat_default_total;
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C) || defined(VARIABLE_C))
extern init_macro_type *macro_info;
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C) || defined(VARIABLE_C))
extern char *macro_name;
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C) || defined(VARIABLE_C))
extern char *macro_text;
#endif
#if (defined(ALLOW_TEMPLATES)) && (defined(INIT1_C) || defined(INIT2_C) || defined(VARIABLE_C))
extern u16b rebuild_raw;
#endif

/* wizard1.c */

#if (defined(ALLOW_SPOILERS)) && (defined(CMD4_C) || defined(OBJECT2_C) || defined(WIZARD1_C) || defined(WIZARD2_C))
extern bool make_fake_artifact(object_type *o_ptr, int name1);
#endif
#if (defined(ALLOW_SPOILERS)) && (defined(DUNGEON_C) || defined(WIZARD1_C))
extern void do_cmd_spoilers(void);
#endif

/* wizard2.c */

#if (defined(DUNGEON_C) || defined(POWERS_C) || defined(WIZARD2_C))
extern void do_cmd_rerate(void);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void do_cmd_wiz_hack_ben(void);
#endif
#if ((defined(ALLOW_WIZARD)) && defined(MONSTER_HORDES)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void do_cmd_summon_horde(void);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void do_cmd_wiz_bamf(void);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void do_cmd_wiz_change(void);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void wiz_create_named_art(int a_idx);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(OBJECT1_C) || defined(WIZARD2_C))
extern void do_cmd_wiz_play(object_type *o_ptr);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void wiz_create_item(int k_idx);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void do_cmd_wiz_cure_all(void);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void do_cmd_wiz_jump(void);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void do_cmd_wiz_learn(void);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void do_cmd_wiz_summon(int num);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void do_cmd_wiz_named(int r_idx, bool slp);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void do_cmd_wiz_named_friendly(int r_idx, bool slp);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void do_cmd_wiz_zap(void);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void do_cmd_magebolt(void);
#endif
#if (defined(ALLOW_WIZARD)) && (defined(DUNGEON_C) || defined(WIZARD2_C))
extern void do_cmd_debug(void);
#endif

/* xtra1.c */

#if (defined(FILES_C) || defined(XTRA1_C))
extern void day_to_date_f1(char *buf, uint max, cptr UNUSED fmt, va_list *vp);
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(XTRA1_C))
extern void cnv_stat_f1(char *buf, uint max, cptr UNUSED fmt, va_list *vp);
#endif
#if (defined(BIRTH_C) || defined(XTRA1_C))
extern s16b modify_stat_value(int value, int amount);
#endif
#if (defined(OBJECT1_C) || defined(XTRA1_C))
extern bool PURE cumber_glove(object_ctype *o_ptr);
#endif
#if (defined(OBJECT1_C) || defined(XTRA1_C))
extern bool PURE cumber_helm(object_ctype *o_ptr);
#endif
#if (defined(OBJECT1_C) || defined(XTRA1_C))
extern int PURE wield_skill(object_ctype *o_ptr);
#endif
#if (defined(XTRA1_C) || defined(XTRA2_C))
extern bool player_no_cut(void);
#endif
#if (defined(XTRA1_C) || defined(XTRA2_C))
extern bool player_no_stun(void);
#endif
#if (defined(DUNGEON_C) || defined(FILES_C) || defined(MONSTER2_C) || defined(STORE_C) || defined(XTRA1_C))
extern bool PURE player_is_undead(void);
#endif
#if (defined(BIRTH_C) || defined(FILES_C) || defined(XTRA1_C))
extern void player_flags(u32b *f1, u32b *f2, u32b *f3);
#endif
#if (defined(CMD2_C) || defined(DUNGEON_C) || defined(SQUELCH_C) || defined(STORE_C) || defined(XTRA1_C))
extern void notice_stuff(void);
#endif
#if (defined(BIRTH_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(SPELLS2_C) || defined(SQUELCH_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void update_stuff(void);
#endif
#if (defined(CMD4_C) || defined(DUNGEON_C) || defined(SQUELCH_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void redraw_stuff(void);
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(INIT2_C) || defined(XTRA1_C))
extern display_func_type display_func[NUM_DISPLAY_FUNCS+1];
#endif
#if (defined(DUNGEON_C) || defined(MAIN_WIN_C) || defined(UTIL_C) || defined(XTRA1_C))
extern void window_stuff(void);
#endif
#if (defined(DUNGEON_C) || defined(XTRA1_C))
extern void toggle_inven_equip(void);
#endif
#if (defined(XTRA1_C) || defined(Z_TERM_C))
extern void event_stuff(void);
#endif
#if (defined(DUNGEON_C) || defined(XTRA1_C))
extern void resize_window(void);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(FILES_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void handle_stuff(void);
#endif
#if (defined(CMD1_C) || defined(FILES_C) || defined(XTRA1_C))
extern bool ma_empty_hands(void);
#endif
#if (defined(FILES_C) || defined(XTRA1_C))
extern bool skill_check_possible(int index);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void skill_exp(int index);
#endif
#if (defined(OBJECT2_C) || defined(SPELLS2_C) || defined(XTRA1_C))
extern int find_object(object_ctype *o_ptr);
#endif
#if (defined(CMD3_C) || defined(SPELLS2_C) || defined(XTRA1_C))
extern void update_objects(int where);
#endif
#if (defined(CMD3_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD2_C) || defined(XTRA1_C))
extern void update_object(object_type *o_ptr);
#endif

/* xtra2.c */

#if (defined(CHECK_ARRAYS)) && (defined(INIT2_C) || defined(XTRA2_C))
extern void check_temp_effects(void);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern bool add_flag(int flag, int v);
#endif
#if (defined(DUNGEON_C) || defined(MELEE2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(STORE_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern bool set_flag(int flag, int v);
#endif
#if (defined(XTRA1_C) || defined(XTRA2_C))
extern cptr PURE prt_flag(int flag);
#endif
#if (defined(CMD4_C) || defined(XTRA2_C))
extern cptr PURE prt_flag_long(int flag);
#endif
#if (defined(XTRA1_C) || defined(XTRA2_C))
extern void prt_timers(void);
#endif
#if (defined(CMD2_C) || defined(XTRA2_C))
extern void gain_exp(s32b amount);
#endif
#if (defined(DUNGEON_C) || defined(POWERS_C) || defined(XTRA2_C))
extern void gain_skills(s32b amount);
#endif
#if (defined(DUNGEON_C) || defined(MELEE1_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C))
extern void lose_skills(s32b amount);
#endif
#if (defined(MELEE2_C) || defined(SPELLS1_C) || defined(XTRA2_C))
extern void monster_death(int m_idx);
#endif
#if (defined(CMD1_C) || defined(MELEE1_C) || defined(SPELLS1_C) || defined(XTRA2_C))
extern bool mon_take_hit(int m_idx, int dam, bool *fear, cptr note);
#endif
#if (defined(CMD3_C) || defined(DUNGEON_C) || defined(XTRA2_C))
extern void panel_bounds(void);
#endif
#if (defined(DUNGEON_C) || defined(XTRA2_C))
extern void panel_bounds_center(void);
#endif
#if (defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(XTRA2_C))
extern void verify_panel(bool force);
#endif
#if (defined(DUNGEON_C) || defined(XTRA2_C))
extern void resize_map(void);
#endif
#if (defined(CMD4_C) || defined(CMD5_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(XTRA2_C))
extern void resize_inkey(void);
#endif
#if (defined(CMD3_C) || defined(CMD4_C) || defined(OBJECT2_C) || defined(SQUELCH_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void ang_sort(vptr u, vptr v, int n);
#endif



#if (defined(CMD3_C) || defined(XTRA2_C))
extern bool target_set(int mode);
#endif
#if (defined(CMD1_C) || defined(CMD6_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern bool get_aim_dir(int *dp);
#endif
#if (defined(DUNGEON_C) || defined(XTRA2_C))
extern bool get_hack_dir(int *dp);
#endif
#if (defined(CMD2_C) || defined(POWERS_C) || defined(XTRA2_C))
extern bool get_rep_dir(int *dp);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(XTRA2_C))
extern bool get_rep_target(int *x, int *y);
#endif
#if (defined(CMD1_C) || defined(POWERS_C) || defined(SPELLS2_C) || defined(WIZARD2_C) || defined(XTRA2_C))
extern bool get_dir_target(int *x, int *y, int dir);
#endif
#if (defined(MONSTER2_C) || defined(XTRA2_C))
extern void convert_articles(char *str);
#endif
#if (defined(CMD2_C) || defined(CMD4_C) || defined(STORE_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void feature_desc_f2(char *buf, uint max, cptr UNUSED fmt, va_list *vp);
#endif
#if (defined(DUNGEON_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern void gain_level_reward(int chosen_reward);
#endif
#if (defined(INIT2_C) || defined(XTRA2_C))
extern void init_chaos(void);
#endif
#if (defined(FILES_C) || defined(POWERS_C) || defined(XTRA2_C))
extern bool PURE p_mutated(void);
#endif
#if (defined(BIRTH_C) || defined(POWERS_C) || defined(XTRA2_C))
extern void p_clear_mutations(void);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(XTRA1_C) || defined(XTRA2_C))
extern bool PURE p_has_mutation(int idx);
#endif
#if (defined(DUNGEON_C) || defined(MONSTER2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(XTRA2_C))
extern bool gain_chaos_feature(int choose_mut);
#endif
#if (defined(DUNGEON_C) || defined(XTRA2_C))
extern bool lose_chaos_feature(int choose_mut);
#endif
#if (defined(SPELLS2_C) || defined(XTRA2_C))
extern int add_chaos_features(cptr *info, bool (*reject)(int));
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(XTRA2_C))
extern void dump_chaos_features(FILE * OutFile);
#endif

/* z-form.c */

#if (defined(UTIL_C) || defined(Z_FORM_C))
extern uint vstrnfmt(char *buf, uint max, cptr fmt, va_list vp);
#endif
#if (defined(XTRA1_C) || defined(Z_FORM_C))
extern void vstrnfmt_fn(char *buf, uint max, cptr UNUSED fmt, va_list *vp);
#endif
#if (defined(OBJECT1_C) || defined(UTIL_C) || defined(Z_FORM_C))
extern char *vformat(cptr fmt, va_list vp);
#endif
#if (defined(BIRTH_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_GTK_C) || defined(MAIN_IBM_C) || defined(MAIN_LSL_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(SQUELCH_C) || defined(UTIL_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA2_C) || defined(Z_FORM_C))
extern uint strnfmt(char *buf, uint max, cptr fmt, ...);
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT1_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MONSTER1_C) || defined(MONSTER2_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C) || defined(Z_FORM_C))
extern char *format(cptr fmt, ...);
#endif
#if (defined(INIT2_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_GCU_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(Z_FORM_C))
extern void plog_fmt(cptr fmt, ...);
#endif
#if (defined(CMD5_C) || defined(CMD6_C) || defined(FILES_C) || defined(INIT1_C) || defined(INIT2_C) || defined(MAID_X11_C) || defined(MAIN_DOS_C) || defined(MAIN_GTK_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(XTRA2_C) || defined(Z_FORM_C))
extern void quit_fmt(cptr fmt, ...);
#endif
#if (defined(FILES_C) || defined(Z_FORM_C))
extern void core_fmt(cptr fmt, ...);
#endif

/* z-rand.c */

#if (defined(TABLES_C) || defined(Z_RAND_C))
extern bool rand_unbiased;
#endif
#if (defined(DUNGEON_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(OBJECT1_C) || defined(Z_RAND_C))
extern bool Rand_quick;
#endif
#if (defined(GENERATE_C) || defined(OBJECT1_C) || defined(Z_RAND_C))
extern u32b Rand_value;
#endif
#if (defined(LOAD_C) || defined(SAVE_C) || defined(Z_RAND_C))
extern u16b Rand_place;
#endif
#if (defined(LOAD_C) || defined(SAVE_C) || defined(Z_RAND_C))
extern u32b Rand_state[RAND_DEG];
#endif
#if (defined(DUNGEON_C) || defined(Z_RAND_C))
extern void Rand_state_init(u32b seed);
#endif
#if (defined(BIRTH_C) || defined(GENERATE_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(Z_RAND_C))
extern s16b randnor(int mean, int stand);
#endif
#if (defined(ANGBAND_H) || defined(Z_RAND_H) || defined(Z_RAND_C))
extern s32b rand_int(u32b m);
#endif
#if (defined(CMD1_C) || defined(CMD2_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT2_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C) || defined(Z_RAND_C))
extern s16b damroll(int num, int sides);
#endif
#if (defined(MONSTER2_C) || defined(Z_RAND_C))
extern s16b maxroll(int num, int sides);
#endif
#if (defined(MAIN_DOS_C) || defined(MAIN_WIN_C) || defined(Z_RAND_C))
extern u32b Rand_simple(u32b m);
#endif

/* z-term.c */

#if (defined(ANGBAND_H) || defined(Z_TERM_C))
extern term *Term;
#endif
#if (defined(DUNGEON_C) || defined(Z_TERM_C))
extern void Term_user(void);
#endif
#if (defined(BIRTH_C) || defined(CMD1_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_GCU_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(UTIL_C) || defined(XTRA1_C) || defined(Z_TERM_C))
extern void Term_xtra(int n, int v);
#endif
#if (defined(CAVE_C) || defined(Z_TERM_C))
extern void Term_queue_char(int x, int y, byte a, char c, byte ta, char tc);
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(POWERS_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(SQUELCH_C) || defined(UTIL_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C) || defined(Z_TERM_C))
extern void Term_fresh(void);
#endif
#if (defined(CAVE_C) || defined(DUNGEON_C) || defined(MAIN_ROS_C) || defined(UTIL_C) || defined(Z_TERM_C))
extern void Term_set_cursor(bool v);
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(CMD4_C) || defined(FILES_C) || defined(MAIN_ROS_C) || defined(MONSTER1_C) || defined(SQUELCH_C) || defined(UTIL_C) || defined(Z_TERM_C))
extern errr Term_gotoxy(int x, int y);
#endif
#if (defined(CAVE_C) || defined(CMD4_C) || defined(OBJECT1_C) || defined(Z_TERM_C))
extern void Term_draw(int x, int y, byte a, char c);
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(CMD4_C) || defined(UTIL_C) || defined(XTRA2_C) || defined(Z_TERM_C))
extern errr Term_addch(byte a, char c);
#endif
#if (defined(BIRTH_C) || defined(CMD3_C) || defined(CMD4_C) || defined(UTIL_C) || defined(XTRA2_C) || defined(Z_TERM_C))
extern errr Term_addstr(int n, byte a, cptr s);
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(CMD4_C) || defined(FILES_C) || defined(WIZARD2_C) || defined(Z_TERM_C))
extern void Term_putch(int x, int y, byte a, char c);
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(MAIN_ROS_C) || defined(UTIL_C) || defined(Z_TERM_C))
extern void Term_putstr(int x, int y, int n, byte a, cptr s);
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(INIT2_C) || defined(MAIN_ROS_C) || defined(MONSTER1_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(UTIL_C) || defined(Z_TERM_C))
extern errr Term_erase(int x, int y, int n);
#endif
#if (defined(BIRTH_C) || defined(CAVE_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(SQUELCH_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(Z_TERM_C))
extern errr Term_clear(void);
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_XAW_C) || defined(Z_TERM_C))
extern errr Term_redraw(void);
#endif
#if (defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(Z_TERM_C))
extern errr Term_redraw_section(int x1, int y1, int x2, int y2);
#endif
#if (defined(CAVE_C) || defined(MAIN_ROS_C) || defined(UTIL_C) || defined(Z_TERM_C))
extern void Term_get_cursor(bool *v);
#endif
#if (defined(CAVE_C) || defined(CMD4_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(SQUELCH_C) || defined(UTIL_C) || defined(XTRA1_C) || defined(XTRA2_C) || defined(Z_TERM_C))
extern void Term_get_size(int *w, int *h);
#endif
#if (defined(STORE_C) || defined(UTIL_C) || defined(Z_TERM_C))
extern errr Term_locate(int *x, int *y);
#endif
#if (defined(CMD4_C) || defined(FILES_C) || defined(MAIN_X11_C) || defined(UTIL_C) || defined(XTRA2_C) || defined(Z_TERM_C))
extern void Term_what(int x, int y, byte *a, char *c);
#endif
#if (defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(UTIL_C) || defined(Z_TERM_C))
extern void Term_flush(void);
#endif
#if (defined(MAIN_AMI_C) || defined(MAIN_CAP_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_EMX_C) || defined(MAIN_GCU_C) || defined(MAIN_GTK_C) || defined(MAIN_IBM_C) || defined(MAIN_LSL_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_SLA_C) || defined(MAIN_VCS_C) || defined(MAIN_VME_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(UTIL_C) || defined(Z_TERM_C))
extern errr Term_keypress(int k);
#endif
#if (defined(CMD4_C) || defined(CMD5_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(UTIL_C) || defined(XTRA2_C) || defined(Z_TERM_C))
extern errr Term_key_push(int k);
#endif
#if (defined(UTIL_C) || defined(Z_TERM_C))
extern int Term_queue_space(void);
#endif
#if (defined(MAIN_AMI_C) || defined(UTIL_C) || defined(Z_TERM_C))
extern errr Term_inkey(char *ch, bool wait, bool take);
#endif
#if (defined(INIT2_C) || defined(Z_TERM_C))
extern void init_term_wins(void);
#endif
#if (defined(CMD4_C) || defined(CMD5_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(WIZARD2_C) || defined(Z_TERM_C))
extern void Term_release(int win);
#endif
#if (defined(CMD4_C) || defined(CMD5_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(WIZARD2_C) || defined(Z_TERM_C))
extern int Term_save_aux(void);
#endif
#if (defined(ANGBAND_H) || defined(Z_TERM_C))
extern void Term_save(void);
#endif
#if (defined(CMD4_C) || defined(CMD5_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(WIZARD2_C) || defined(Z_TERM_C))
extern void Term_load_aux(int win);
#endif
#if (defined(ANGBAND_H) || defined(Z_TERM_C))
extern void Term_load(void);
#endif
#if (defined(MAIN_AMI_C) || defined(MAIN_CRB_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(Z_TERM_C))
extern void Term_resize(int w, int h);
#endif
#if (defined(CMD4_C) || defined(MAIN_AMI_C) || defined(MAIN_CAP_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_EMX_C) || defined(MAIN_GCU_C) || defined(MAIN_GTK_C) || defined(MAIN_IBM_C) || defined(MAIN_LSL_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_SLA_C) || defined(MAIN_VCS_C) || defined(MAIN_VME_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(MAIN_XXX_C) || defined(UTIL_C) || defined(XTRA1_C) || defined(Z_TERM_C))
extern void Term_activate(term *t);
#endif
#if (defined(MAIN_DOS_C) || defined(MAIN_EMX_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(Z_TERM_C))
extern void term_nuke(term *t);
#endif
#if (defined(MAIN_AMI_C) || defined(MAIN_CAP_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_EMX_C) || defined(MAIN_GCU_C) || defined(MAIN_GTK_C) || defined(MAIN_IBM_C) || defined(MAIN_LSL_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_SLA_C) || defined(MAIN_VCS_C) || defined(MAIN_VME_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(MAIN_XXX_C) || defined(Z_TERM_C))
extern void term_init(term *t, int w, int h, int k);
#endif

/* z-util.c */

#if (defined(MAIN_EMX_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(Z_UTIL_C))
extern cptr argv0;
#endif
#if (defined(XTRA1_C) || defined(Z_UTIL_C))
extern void func_nothing(void);
#endif
#if (defined(XTRA1_C) || defined(Z_UTIL_C))
extern bool func_true(void);
#endif
#if (defined(XTRA1_C) || defined(Z_UTIL_C))
extern bool func_false(void);
#endif
#if (defined(MAIN_DOS_C) || defined(MAIN_EMX_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(Z_UTIL_C))
extern bool suffix(cptr s, cptr t);
#endif
#if (defined(CAVE_C) || defined(CMD5_C) || defined(FILES_C) || defined(INIT1_C) || defined(MAIN_GCU_C) || defined(MAIN_GTK_C) || defined(MAIN_ROS_C) || defined(MAIN_X11_C) || defined(MAIN_XAW_C) || defined(MAIN_XPJ_C) || defined(OBJECT1_C) || defined(UTIL_C) || defined(Z_UTIL_C))
extern bool prefix(cptr s, cptr t);
#endif
#if (defined(MAIN_CRB_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(Z_UTIL_C))
extern void (*plog_aux)(cptr);
#endif
#if (defined(INIT2_C) || defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(Z_FORM_C) || defined(Z_UTIL_C))
extern void plog(cptr str);
#endif
#if (defined(MAIN_CRB_C) || defined(MAIN_DOS_C) || defined(MAIN_EMX_C) || defined(MAIN_GCU_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(Z_UTIL_C))
extern void (*quit_aux)(cptr);
#endif
#if (defined(BIRTH_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(MAIN_CAP_C) || defined(MAIN_CRB_C) || defined(MAIN_EMX_C) || defined(MAIN_GCU_C) || defined(MAIN_GTK_C) || defined(MAIN_IBM_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_SLA_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XPJ_C) || defined(MAIN_XXX_C) || defined(MAIN_C) || defined(SAVE_C) || defined(UTIL_C) || defined(XTRA2_C) || defined(Z_FORM_C) || defined(Z_UTIL_C) || defined(Z_VIRT_C))
extern void quit(cptr str);
#endif
#if (defined(MAIN_CRB_C) || defined(MAIN_GCU_C) || defined(MAIN_GTK_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(MAIN_WIN_C) || defined(Z_UTIL_C))
extern void (*core_aux)(cptr);
#endif
#if (defined(MAIN_ROS_C) || defined(MAIN_XAW_C) || defined(UTIL_C) || defined(Z_FORM_C) || defined(Z_UTIL_C) || defined(Z_VIRT_C))
extern void core(cptr str);
#endif

/* z-virt.c */

#if (defined(MAIN_CRB_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(Z_VIRT_C))
extern vptr (*rnfree_aux)(vptr);
#endif
#if (defined(Z_VIRT_H) || defined(Z_VIRT_C))
extern vptr rnfree(vptr p);
#endif
#if (defined(MONSTER2_C) || defined(OBJECT2_C) || defined(Z_VIRT_C))
extern vptr rpanic_none(huge UNUSED len);
#endif
#if (defined(MAIN_CRB_C) || defined(MAIN_MAC_C) || defined(MONSTER2_C) || defined(OBJECT2_C) || defined(Z_VIRT_C))
extern vptr (*rpanic_aux)(huge);
#endif
#if (defined(MAIN_CRB_C) || defined(MAIN_MAC_C) || defined(MAIN_ROS_C) || defined(Z_VIRT_C))
extern vptr (*ralloc_aux)(huge);
#endif
#if (defined(ANGBAND_H) || defined(Z_VIRT_H) || defined(Z_VIRT_C))
extern vptr ralloc(huge len);
#endif
#if (defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(INIT1_C) || defined(INIT2_C) || defined(MAIN_GTK_C) || defined(MAIN_LSL_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XPJ_C) || defined(MAIN_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(UTIL_C) || defined(Z_VIRT_C))
extern cptr string_make(cptr str);
#endif
#if (defined(DUNGEON_C) || defined(LOAD_C) || defined(OBJECT1_C) || defined(SPELLS2_C) || defined(Z_VIRT_C))
extern void safe_free(vptr p);
#endif
#if (defined(LOAD_C) || defined(OBJECT1_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(Z_VIRT_C))
extern cptr safe_string_make(cptr str);
#endif

/* cave.c */

#if (defined(CAVE_C) || defined(MELEE2_C) || defined(WIZARD2_C))
extern bool move_in_direction(int *xx, int *yy, int x1, int y1, int x2, int y2, bool (*okay)(int, int, int));
#endif

/* cmd3.c */

#if (defined(CMD3_C) || defined(OBJECT1_C))
extern bool PURE item_tester_hook_wear(object_ctype *o_ptr);
#endif

/* object1.c */

#if (defined(DUNGEON_C) || defined(OBJECT1_C))
extern bool do_cmd_use_object(s16b cmd);
#endif
#if (defined(OBJECT1_C) || defined(SPELLS2_C))
extern bool PURE item_tester_hook_destroy(object_ctype *o_ptr);
#endif
#if (defined(CMD1_C) || defined(OBJECT1_C) || defined(SPELLS2_C))
extern object_type *get_object_from_function(void (*func)(object_type *));
#endif

/* spells2.c */

#if (defined(OBJECT1_C) || defined(SPELLS2_C))
extern void do_identify_fully(object_type *o_ptr);
#endif

/* util.c */

#if (defined(INIT2_C) || defined(UTIL_C))
extern void init_ascii_text_conv(void);
#endif
#if (defined(DUNGEON_C) || defined(OBJECT1_C) || defined(UTIL_C))
extern void s16b_to_string_f1(char *buf, uint max, cptr UNUSED fmt, va_list *vp);
#endif
#endif /* INCLUDED_EXTERNS_H */
