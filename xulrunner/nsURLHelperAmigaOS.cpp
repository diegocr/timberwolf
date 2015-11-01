/* ***** BEGIN LICENSE BLOCK *****
 *
 * The contents of this file is copyrighted by Thomas and Hans-Joerg Frieden.
 * It's content is not open source and may not be redistributed, modified or adapted
 * without permission of the above-mentioned copyright holders.
 *
 * Since this code was originally developed under an AmigaOS related bounty, any derived
 * version of this file may only be used on an official AmigaOS system.
 *
 * Contributor(s):
 * 	Thomas Frieden <thomas@friedenhq.org>
 * 	Hans-Joerg Frieden <hans-joerg@friedenhq.org>
 *
 * ***** END LICENSE BLOCK ***** */


/* AmigaOS-specific local file uri parsing */
#include "nsURLHelper.h"
#include "nsEscape.h"
#include "nsILocalFile.h"
#include "nsNativeCharsetUtils.h"

nsresult
net_GetURLSpecFromFile(nsIFile *aFile, nsACString &result)
{
    nsresult rv;
    nsCAutoString nativePath, ePath;
    nsAutoString path;

    rv = aFile->GetNativePath(nativePath);
    if (NS_FAILED(rv)) return rv;

    // Convert to unicode and back to check correct conversion to native charset
    NS_CopyNativeToUnicode(nativePath, path);
    NS_CopyUnicodeToNative(path, ePath);

    // Use UTF8 version if conversion was successful
    if (nativePath == ePath)
        CopyUTF16toUTF8(path, ePath);
    else
        ePath = nativePath;

    nsCAutoString escPath;
    NS_NAMED_LITERAL_CSTRING(prefix, "file:///");

    // Escape the path with the directory mask
    if (NS_EscapeURL(ePath.get(), -1, esc_Directory+esc_Forced, escPath))
        escPath.Insert(prefix, 0);
    else
        escPath.Assign(prefix + ePath);

    // esc_Directory does not escape the semicolons, so if a filename
    // contains semicolons we need to manually escape them.
    escPath.ReplaceSubstring(";", "%3b");

    // if this file references a directory, then we need to ensure that the
    // URL ends with a slash.  this is important since it affects the rules
    // for relative URL resolution when this URL is used as a base URL.
    // if the file does not exist, then we make no assumption about its type,
    // and simply leave the URL unmodified.
    if (escPath.Last() != '/') {
        PRBool dir;
        rv = aFile->IsDirectory(&dir);
        if (NS_SUCCEEDED(rv) && dir)
            escPath += '/';
    }

    result = escPath;
    return NS_OK;
}

nsresult
net_GetFileFromURLSpec(const nsACString &aURL, nsIFile **result)
{
    // NOTE: See also the implementation in nsURLHelperOSX.cpp,
    // which is based on this.
    nsresult rv;

    nsCOMPtr<nsILocalFile> localFile;
    rv = NS_NewNativeLocalFile(EmptyCString(), PR_TRUE, getter_AddRefs(localFile));
    if (NS_FAILED(rv))
      return rv;

    nsCAutoString directory, fileBaseName, fileExtension, path;

    rv = net_ParseFileURL(aURL, directory, fileBaseName, fileExtension);
    if (NS_FAILED(rv)) return rv;

    if (!directory.IsEmpty())
        NS_EscapeURL(directory, esc_Directory|esc_AlwaysCopy, path);
    if (!fileBaseName.IsEmpty())
        NS_EscapeURL(fileBaseName, esc_FileBaseName|esc_AlwaysCopy, path);
    if (!fileExtension.IsEmpty()) {
        path += '.';
        NS_EscapeURL(fileExtension, esc_FileExtension|esc_AlwaysCopy, path);
    }

    NS_UnescapeURL(path);
    if (path.Length() != strlen(path.get()))
        return NS_ERROR_FILE_INVALID_PATH;

    if (path.CharAt(0) == '/')
        path.Cut(0, 1);

    if (IsUTF8(path)) {
        // speed up the start-up where UTF-8 is the native charset
        // (e.g. on recent Linux distributions)
        if (NS_IsNativeUTF8())
            rv = localFile->InitWithNativePath(path);
        else
            rv = localFile->InitWithPath(NS_ConvertUTF8toUTF16(path));
            // XXX In rare cases, a valid UTF-8 string can be valid as a native
            // encoding (e.g. 0xC5 0x83 is valid both as UTF-8 and Windows-125x).
            // However, the chance is very low that a meaningful word in a legacy
            // encoding is valid as UTF-8.
    }
    else
        // if path is not in UTF-8, assume it is encoded in the native charset
        rv = localFile->InitWithNativePath(path);

    if (NS_FAILED(rv)) return rv;

    NS_ADDREF(*result = localFile);
    return NS_OK;
}
