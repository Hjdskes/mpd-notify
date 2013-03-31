pkgname=mpd-notify
pkgver=20120921
pkgrel=1
pkgdesc="Notifies you about MPD"
arch=('i686' 'x86_64')
url="https://github.com/Unia/mpd-notify"
depends=('libnotify' 'libmpdclient')
license=('GPL')

_gitroot="https://github.com/Unia/mpd-notify.git"
_gitname="${pkgname}"

package() {
  cd ${srcdir}/
    msg "Connecting to GIT server...."
    
    if [ -d ${_gitname} ] ; then
        cd ${_gitname} && git pull origin
        msg "The local files are updated."
    else
        git clone ${_gitroot} ${_gitname}
    fi
    msg "GIT checkout done or server timeout"

  cd ${srcdir}/${pkgname}

  make
}

build() {
  cd ${srcdir}/${pkgname}

  make DESTDIR="${pkgdir}" install
}
