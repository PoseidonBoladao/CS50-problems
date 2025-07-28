SELECT AVG(energy) FROM songs JOIN artists WHERE artists.id = songs.artist_id AND artists.name = "Drake";
