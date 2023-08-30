# Image post-processing to setup user accounts

inherit extrausers
 
# Space-delimited list of user:password pairs
NEWUSERS ??= ""

# root password
ROOT_PASSWORD ??= ""

python () {
   params = ""

   # add new users
   newusers = (d.getVar("NEWUSERS", True) or "").split()
   for user in newusers:
      p = user.split(":")
      name = p[0]
      password = p[1]
      groups = ""
      if len(p) >= 3:
         groups = p[2]
      if password == "<system>":
         params += "useradd -r -s /bin/false " + name + "; "
      elif password == "<daemon>":
         params += "useradd -m -r -s /bin/false " + name + "; "
      elif password == "<disabled>":
         params += "useradd -m " + name + "; "
      else:
         params += "useradd -m -p '`openssl passwd -6 " + password + "`' " + name + "; "
      if groups != "":
         params += "usermod -a -G " + groups + " " + name + "; "

   # modify root password
   rootpw = d.getVar("ROOT_PASSWORD", True) or ""
   if rootpw:
      params += "usermod -p '`openssl passwd -6 " + rootpw + "`' root; "

   d.setVar("EXTRA_USERS_PARAMS", params)
}
 
