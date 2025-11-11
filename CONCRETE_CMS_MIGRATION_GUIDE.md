# Concrete CMS v9 Migration Guide: Moving to a New Domain (Plesk + FTP)

This guide provides detailed step-by-step instructions for migrating a Concrete CMS version 9 website from one domain to another using Plesk hosting control panel and FTP.

## Prerequisites

- Access to both source and destination Plesk hosting accounts
- FTP credentials for both servers
- Database access (phpMyAdmin or command line)
- Backup of the current website (recommended)
- SSH access (optional but helpful for some steps)

## Phase 1: Pre-Migration Preparation

### Step 1: Create Full Backup of Source Website

1. **Backup Files via FTP:**
   - Connect to source server via FTP client (FileZilla, WinSCP, etc.)
   - Download entire website directory (usually `httpdocs` or `public_html`)
   - Ensure hidden files (`.htaccess`, `.env`) are included

2. **Backup Database:**
   - Log into Plesk control panel (source domain)
   - Navigate to **Databases** → **phpMyAdmin**
   - Select your Concrete CMS database
   - Click **Export** tab
   - Choose **Quick** export method, format: **SQL**
   - Click **Go** to download the database dump

3. **Document Current Configuration:**
   - Note the current domain name
   - Document database name, username, and password
   - Note PHP version (Plesk → **PHP Settings**)
   - Document any custom server configurations

### Step 2: Prepare Destination Server

1. **Create Domain/Subdomain in Plesk:**
   - Log into Plesk (destination server)
   - Go to **Websites & Domains** → **Add Domain**
   - Enter new domain name
   - Configure DNS settings if needed

2. **Set PHP Version:**
   - Navigate to **PHP Settings** for the new domain
   - Match PHP version to source server (Concrete CMS v9 requires PHP 7.4+)
   - Enable required PHP extensions:
     - `mysqli` or `pdo_mysql`
     - `gd` or `imagick`
     - `zip`
     - `mbstring`
     - `xml`
     - `curl`
     - `openssl`

3. **Create Database:**
   - Go to **Databases** → **Add Database**
   - Create new database (note name, username, password)
   - Grant all privileges to the database user

## Phase 2: File Transfer

### Step 3: Upload Files to Destination Server

1. **Connect via FTP:**
   - Use FTP client to connect to destination server
   - Navigate to `httpdocs` or `public_html` directory

2. **Upload Files:**
   - Upload all files from your backup
   - Maintain directory structure exactly as source
   - Ensure file permissions are correct:
     - Directories: `755` or `750`
     - Files: `644` or `640`
     - `concrete/` directory: `755`
     - `application/` directory: `755`
     - `packages/` directory: `755`

3. **Verify Critical Files:**
   - Ensure `.htaccess` file is uploaded
   - Check `concrete/config/database.php` exists
   - Verify `application/config/site.php` exists

### Step 4: Restore Database

1. **Import Database:**
   - In Plesk, go to **Databases** → **phpMyAdmin**
   - Select the new database
   - Click **Import** tab
   - Choose the SQL file from your backup
   - Click **Go** to import

2. **Verify Import:**
   - Check that all tables are imported
   - Verify table count matches source database

## Phase 3: Configuration Updates

### Step 5: Update Database Configuration

1. **Update Database Credentials:**
   - Via FTP, download `concrete/config/database.php`
   - Edit the file with new database credentials:
   ```php
   return [
       'default-connection' => 'concrete',
       'connections' => [
           'concrete' => [
               'driver' => 'c5_pdo_mysql',
               'server' => 'localhost',
               'database' => 'NEW_DATABASE_NAME',
               'username' => 'NEW_DATABASE_USER',
               'password' => 'NEW_DATABASE_PASSWORD',
               'charset' => 'utf8mb4',
           ],
       ],
   ];
   ```
   - Upload the modified file back to server

2. **Alternative: Use Plesk File Manager:**
   - In Plesk, go to **Files** → **File Manager**
   - Navigate to `concrete/config/`
   - Edit `database.php` directly in Plesk editor
   - Save changes

### Step 6: Update Site Configuration

1. **Update Base URL:**
   - Edit `application/config/site.php`:
   ```php
   return [
       'site' => 'NEW_DOMAIN_NAME',
       'canonical_url' => 'https://NEW_DOMAIN_NAME',
       'canonical_url_alternative' => 'http://NEW_DOMAIN_NAME',
   ];
   ```

2. **Update .htaccess (if needed):**
   - Check `.htaccess` for any hardcoded domain references
   - Update rewrite rules if necessary

### Step 7: Update Database Content

1. **Update Domain References in Database:**
   - Access phpMyAdmin for new database
   - Run SQL queries to update domain references:
   ```sql
   -- Update site configuration
   UPDATE Config SET configValue = 'https://NEW_DOMAIN_NAME' WHERE configKey = 'SITE';
   
   -- Update canonical URLs
   UPDATE Config SET configValue = 'https://NEW_DOMAIN_NAME' WHERE configKey LIKE '%canonical%';
   
   -- Update file manager paths (if needed)
   UPDATE FileStorageLocations SET fslPath = REPLACE(fslPath, 'OLD_DOMAIN', 'NEW_DOMAIN');
   ```

2. **Update File Paths (if absolute paths were used):**
   ```sql
   -- Check for old domain in various tables
   SELECT * FROM FileVersions WHERE fvFilename LIKE '%OLD_DOMAIN%';
   SELECT * FROM Pages WHERE cName LIKE '%OLD_DOMAIN%';
   ```

### Step 8: Clear Concrete CMS Cache

1. **Via FTP/File Manager:**
   - Delete cache directories:
     - `application/files/cache/`
     - `application/files/tmp/`
   - Keep directory structure, just remove contents

2. **Via Command Line (if SSH available):**
   ```bash
   cd /path/to/concrete/cms
   php concrete/bin/concrete5 c5:clear-cache
   ```

## Phase 4: Post-Migration Verification

### Step 9: Test Website Functionality

1. **Basic Access:**
   - Visit new domain in browser
   - Check if homepage loads correctly
   - Verify no 404 errors

2. **Admin Access:**
   - Navigate to `/index.php/login`
   - Log in with admin credentials
   - Verify dashboard loads

3. **Check Key Features:**
   - Test page navigation
   - Verify images/files load correctly
   - Test forms (if any)
   - Check file manager
   - Verify custom themes/styles load

### Step 10: Update DNS (If Applicable)

1. **DNS Configuration:**
   - If moving to completely new domain, update DNS records:
     - A record: Point to new server IP
     - CNAME records: Update subdomains
   - Wait for DNS propagation (can take 24-48 hours)

2. **Temporary Access:**
   - While DNS propagates, you can access via:
     - IP address (if allowed)
     - Hosts file modification (local testing)
     - Temporary subdomain

### Step 11: SSL Certificate Setup

1. **Install SSL Certificate:**
   - In Plesk, go to **SSL/TLS Certificates**
   - Install Let's Encrypt certificate (free) or upload your own
   - Enable **Force HTTPS** redirect

2. **Update Concrete CMS for HTTPS:**
   - Ensure site configuration uses `https://` URLs
   - Update `.htaccess` for HTTPS redirects if needed

## Phase 5: Final Steps

### Step 12: Update Search Engine Settings

1. **Update Sitemap:**
   - Regenerate sitemap in Concrete CMS
   - Submit to Google Search Console (new property)

2. **301 Redirects (Old Domain):**
   - Set up redirects on old domain to new domain:
   ```apache
   # In old domain's .htaccess
   RewriteEngine On
   RewriteCond %{HTTP_HOST} ^OLD_DOMAIN$ [OR]
   RewriteCond %{HTTP_HOST} ^www.OLD_DOMAIN$
   RewriteRule ^(.*)$ https://NEW_DOMAIN/$1 [R=301,L]
   ```

### Step 13: Update External Services

1. **Email Configuration:**
   - Update email settings if domain-specific
   - Test email functionality

2. **Third-Party Integrations:**
   - Update API endpoints if domain-specific
   - Update webhook URLs
   - Update OAuth redirect URIs

3. **CDN/Media Services:**
   - Update CDN origin URLs
   - Update media library paths if using external storage

## Troubleshooting Common Issues

### Issue: White Screen / 500 Error
**Solution:**
- Check PHP error logs in Plesk → **Logs**
- Verify file permissions
- Check `.htaccess` syntax
- Ensure PHP version compatibility

### Issue: Database Connection Error
**Solution:**
- Verify `database.php` credentials
- Check database user has proper permissions
- Ensure database server is `localhost` (or correct host)

### Issue: Images/Files Not Loading
**Solution:**
- Check file permissions (`755` for directories, `644` for files)
- Verify file paths in database
- Clear Concrete CMS cache
- Check `.htaccess` rewrite rules

### Issue: Admin Login Not Working
**Solution:**
- Verify database was imported correctly
- Check user table exists: `SELECT * FROM Users;`
- Try password reset if needed
- Check session configuration

### Issue: Mixed Content (HTTP/HTTPS)
**Solution:**
- Update all URLs to use `https://`
- Check database for hardcoded `http://` URLs
- Update site configuration
- Use browser developer tools to identify mixed content sources

## Important Notes

1. **File Permissions:**
   - Concrete CMS requires specific permissions
   - `application/files/` must be writable (`755` or `775`)
   - `concrete/` directory should be readable (`755`)

2. **PHP Settings:**
   - Ensure `memory_limit` is at least `128M` (recommended `256M`)
   - `max_execution_time` should be adequate for imports
   - `upload_max_filesize` and `post_max_size` should match your needs

3. **Database Character Set:**
   - Ensure database uses `utf8mb4` character set
   - Verify collation is `utf8mb4_unicode_ci`

4. **Backup Before Changes:**
   - Always keep backups until migration is fully verified
   - Test on staging environment if possible

## Quick Reference Checklist

- [ ] Backup source website files
- [ ] Backup source database
- [ ] Create new domain in Plesk
- [ ] Set PHP version and extensions
- [ ] Create new database
- [ ] Upload files via FTP
- [ ] Import database
- [ ] Update `database.php` configuration
- [ ] Update `site.php` configuration
- [ ] Update domain references in database
- [ ] Clear Concrete CMS cache
- [ ] Test website functionality
- [ ] Test admin login
- [ ] Install SSL certificate
- [ ] Set up 301 redirects (old domain)
- [ ] Update DNS records
- [ ] Update external services/integrations
- [ ] Verify all features work
- [ ] Monitor for errors

## Additional Resources

- Concrete CMS Documentation: https://documentation.concretecms.org/
- Concrete CMS v9 Release Notes: Check official website
- Plesk Documentation: https://docs.plesk.com/

---

**Migration Date:** _______________
**Source Domain:** _______________
**Destination Domain:** _______________
**Database Name:** _______________
**Notes:** _______________
