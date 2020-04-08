DROP TABLE IF EXISTS `pn_mac`;
DROP TABLE IF EXISTS `reports`;
DROP TABLE IF EXISTS `mac_founds`;

CREATE TABLE IF NOT EXISTS `pn_mac` (
	`id` int(10) unsigned NOT NULL AUTO_INCREMENT,
	`phone` varchar(64) NOT NULL,
	`mac` varchar(64) NOT NULL,
	`created` DATETIME DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY (`id`),
	KEY `phone` (`phone`),
	KEY `mac` (`mac`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `reports` (
	`id` int(10) unsigned NOT NULL AUTO_INCREMENT,
	`zone` varchar(64) NOT NULL,
	`mac` varchar(64),
	`rssi` int(64),
	`created` DATETIME DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY (`id`),
	KEY `zone` (`zone`),
	KEY `mac` (`mac`),
	KEY `rssi` (`rssi`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `mac_founds` (
	`id` int(10) unsigned NOT NULL AUTO_INCREMENT,
	`store` varchar(64) NOT NULL,
	`mac` varchar(64) UNIQUE,
	`phone` varchar(64),
	`rssi` int(64),
	`created` DATETIME DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY (`id`),
	KEY `store` (`store`),
	KEY `phone` (`phone`),
	KEY `rssi` (`rssi`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
