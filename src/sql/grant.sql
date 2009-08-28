CREATE USER "__WWWUSER__";

GRANT ALL ON images TO GROUP observers;
GRANT ALL ON counts TO GROUP observers;
GRANT ALL ON users TO GROUP observers;
GRANT ALL ON medias TO GROUP observers;
GRANT ALL ON grb TO GROUP observers;
GRANT ALL ON grb_gcn TO GROUP observers;
GRANT ALL ON grb_script TO GROUP observers;
GRANT ALL ON swift TO GROUP observers;
GRANT ALL ON integral TO GROUP observers;
GRANT ALL ON targets TO GROUP observers;
GRANT ALL ON observations TO GROUP observers;
GRANT ALL ON ot TO GROUP observers;
GRANT ALL ON ell TO GROUP observers;
GRANT ALL ON terestial TO GROUP observers;
GRANT ALL ON cameras TO GROUP observers;
GRANT ALL ON mounts TO GROUP observers;
GRANT ALL ON counters TO GROUP observers;
GRANT ALL ON darks TO GROUP observers;
GRANT ALL ON flats TO GROUP observers;
GRANT ALL ON types TO GROUP observers;
GRANT ALL ON phot TO GROUP observers;
GRANT ALL ON target_model TO GROUP observers;
GRANT ALL ON model_observation TO GROUP observers;
GRANT ALL ON medias TO GROUP observers;
GRANT ALL ON epoch TO GROUP observers;
GRANT ALL ON plan TO GROUP observers;
GRANT ALL ON props TO GROUP observers;
GRANT ALL ON filters TO GROUP observers;
GRANT ALL ON calibration_observation TO GROUP observers;
GRANT SELECT,UPDATE ON obs_id TO GROUP observers;
GRANT SELECT,UPDATE ON img_id TO GROUP observers;
GRANT SELECT,UPDATE ON med_id TO GROUP observers;
GRANT SELECT,UPDATE ON tar_id TO GROUP observers;
GRANT SELECT,UPDATE ON user_id TO GROUP observers;
GRANT SELECT,UPDATE ON plan_id TO GROUP observers;
GRANT SELECT,UPDATE ON prop_id TO GROUP observers;
GRANT SELECT,UPDATE ON grb_tar_id TO GROUP observers;
GRANT SELECT,UPDATE ON point_id TO GROUP observers;
GRANT SELECT,UPDATE ON med_id TO "__WWWUSER__";
GRANT SELECT,UPDATE ON tar_id TO "__WWWUSER__";
GRANT SELECT,UPDATE ON user_id TO "__WWWUSER__";
GRANT SELECT,UPDATE ON plan_id TO "__WWWUSER__";
GRANT SELECT,UPDATE ON plan TO "__WWWUSER__";
GRANT SELECT ON grb_tar_id TO "__WWWUSER__";
GRANT SELECT ON images_nights TO GROUP observers;
GRANT SELECT ON observations_nights TO GROUP observers;
GRANT SELECT ON images TO "__WWWUSER__";
GRANT SELECT ON counts TO "__WWWUSER__";
GRANT SELECT ON scripts TO "__WWWUSER__";
GRANT SELECT ON users TO "__WWWUSER__";
GRANT SELECT ON epoch TO "__WWWUSER__";
GRANT SELECT ON flats TO "__WWWUSER__";
GRANT UPDATE ON images TO "__WWWUSER__";
GRANT ALL ON medias TO "__WWWUSER__";
GRANT ALL ON images TO GROUP observers;
GRANT ALL ON counts TO GROUP observers;
GRANT ALL ON users TO GROUP observers;
GRANT ALL ON medias TO GROUP observers;
GRANT ALL ON grb TO "__WWWUSER__";
GRANT SELECT ON grb_gcn TO "__WWWUSER__";
GRANT SELECT ON grb_script TO "__WWWUSER__";
GRANT SELECT ON swift TO "__WWWUSER__";
GRANT SELECT ON integral TO "__WWWUSER__";
GRANT ALL ON targets TO "__WWWUSER__";
GRANT ALL ON scripts TO GROUP observers;
GRANT SELECT ON observations TO "__WWWUSER__";
GRANT SELECT ON swift_observation TO "__WWWUSER__";
GRANT SELECT ON integral_observation TO "__WWWUSER__";
GRANT ALL ON ot TO "__WWWUSER__";
GRANT ALL ON ell TO "__WWWUSER__";
GRANT ALL ON terestial TO "__WWWUSER__";
GRANT SELECT ON cameras TO "__WWWUSER__";
GRANT SELECT ON mounts TO "__WWWUSER__";
GRANT SELECT ON counters TO "__WWWUSER__";
GRANT SELECT ON types TO "__WWWUSER__";
GRANT SELECT ON phot TO "__WWWUSER__";
GRANT SELECT ON target_model TO "__WWWUSER__";
GRANT SELECT ON model_observation TO "__WWWUSER__";
GRANT SELECT ON medias TO "__WWWUSER__";
GRANT ALL ON grb TO GROUP observers;
GRANT ALL ON grb_gcn TO GROUP observers;
GRANT ALL ON grb_script TO GROUP observers;
GRANT ALL ON swift TO GROUP observers;
GRANT ALL ON integral TO GROUP observers;
GRANT ALL ON targets TO GROUP observers;
GRANT ALL ON observations TO GROUP observers;
GRANT ALL ON swift_observation TO GROUP observers;
GRANT ALL ON integral_observation TO GROUP observers;
GRANT ALL ON ot TO GROUP observers;
GRANT ALL ON cameras TO GROUP observers;
GRANT ALL ON mounts TO GROUP observers;
GRANT ALL ON darks TO GROUP observers;
GRANT ALL ON types TO GROUP observers; 
GRANT ALL ON medias TO GROUP observers;
GRANT SELECT ON images_nights TO GROUP observers;
GRANT SELECT ON observations_nights TO GROUP observers;
GRANT SELECT ON targets_noimages TO GROUP observers;
GRANT SELECT ON targets_imgcount TO GROUP observers;
GRANT SELECT ON targets_images TO GROUP observers;
GRANT SELECT ON observations_noimages TO GROUP observers;
GRANT SELECT ON observations_imgcount TO GROUP observers;
GRANT SELECT ON observations_images TO GROUP observers;
GRANT SELECT ON observations_nights TO GROUP observers;
GRANT SELECT ON images_nights TO GROUP observers;
GRANT SELECT ON targets_noimages TO "__WWWUSER__";
GRANT SELECT ON targets_imgcount TO "__WWWUSER__";
GRANT SELECT ON targets_images TO "__WWWUSER__";
GRANT SELECT ON targets_enabled TO "__WWWUSER__";
GRANT SELECT ON targets_counts TO "__WWWUSER__";
GRANT SELECT ON observations_noimages TO "__WWWUSER__";
GRANT SELECT ON observations_imgcount TO "__WWWUSER__";
GRANT SELECT ON observations_images TO "__WWWUSER__";
GRANT SELECT ON observations_nights TO "__WWWUSER__";
GRANT SELECT ON images_nights TO "__WWWUSER__";
GRANT SELECT ON targets_noimages TO GROUP observers;
GRANT SELECT ON targets_imgcount TO GROUP observers;
GRANT SELECT ON targets_images TO GROUP observers;
GRANT SELECT ON targets_enabled TO GROUP observers;
GRANT SELECT ON targets_counts TO GROUP observers;
GRANT SELECT ON observations_noimages TO GROUP observers;
GRANT SELECT ON observations_imgcount TO GROUP observers;
GRANT SELECT ON observations_images TO GROUP observers;
GRANT SELECT ON observations_nights TO GROUP observers;
GRANT SELECT ON images_nights TO GROUP observers;

GRANT ALL ON airmass_cal_images TO GROUP observers;
GRANT SELECT ON airmass_cal_images TO "__WWWUSER__";

GRANT ALL ON targets_users TO GROUP observers;
GRANT ALL ON targets_users TO "__WWWUSER__";

GRANT ALL ON type_users TO GROUP observers;
GRANT ALL ON type_users TO "__WWWUSER__";

GRANT ALL ON message TO GROUP observers;
GRANT SELECT ON message TO "__WWWUSER__";
