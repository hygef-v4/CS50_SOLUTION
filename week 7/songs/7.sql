-- avg energy of songs that are by Drake
SELECT AVG(energy) FROM songs
WHERE artist_id = (SELECT id from artists WHERE name = "Drake");
